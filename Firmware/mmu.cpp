//mmu.cpp

#include "mmu.h"
#include "planner.h"
#include "language.h"
#include "lcd.h"

#ifdef INCLUDE_UART2
	#include "uart2.h"
#endif // INCLUDE_UART2

#include "temperature.h"
#include "Configuration_prusa.h"


extern const char* lcd_display_message_fullscreen_P(const char *msg);
extern void lcd_show_fullscreen_message_and_wait_P(const char *msg);
extern int8_t lcd_show_fullscreen_message_yes_no_and_wait_P(const char *msg, bool allow_timeouting = true, bool default_yes = false);
extern void lcd_return_to_status();
extern void lcd_wait_for_heater();
extern char choose_extruder_menu();


#define MMU_TODELAY 100
#define MMU_TIMEOUT 10

#define MMU_HWRESET
#define MMU_RST_PIN 76


bool mmu_enabled = false;

int8_t mmu_state = 0;

uint8_t mmu_extruder = 0;

uint8_t tmp_extruder = 0;

int8_t mmu_finda = -1;

int16_t mmu_version = -1;

int16_t mmu_buildnr = -1;


//clear rx buffer
void mmu_clr_rx_buf(void)
{
	while (fgetc(uart2io) >= 0);
}

//send command - puts
int mmu_puts_P(const char* str)
{
	mmu_clr_rx_buf();                          //clear rx buffer
    return fputs_P(str, uart2io);              //send command
}

//send command - printf
int mmu_printf_P(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	mmu_clr_rx_buf();                          //clear rx buffer
	int r = vfprintf_P(uart2io, format, args); //send command
	va_end(args);
	return r;
}

//check 'ok' response
int8_t mmu_rx_ok(void)
{
	return uart2_rx_str_P(PSTR("ok\n"));
}

//check 'start' response
int8_t mmu_rx_start(void)
{
	return uart2_rx_str_P(PSTR("start\n"));
}

//initialize mmu2 unit - first part - should be done at begining of startup process
void mmu_init(void)
{
	digitalWrite(MMU_RST_PIN, HIGH);
	pinMode(MMU_RST_PIN, OUTPUT);              //setup reset pin
	uart2_init();                              //init uart2
	_delay_ms(10);                             //wait 10ms for sure
	mmu_reset();                               //reset mmu (HW or SW), do not wait for response
	mmu_state = -1;
}

//mmu main loop - state machine processing
void mmu_loop(void)
{
//	printf_P(PSTR("MMU loop, state=%d\n"), mmu_state);
	switch (mmu_state)
	{
	case 0:
		return;
	case -1:
		if (mmu_rx_start() > 0)
		{
			puts_P(PSTR("MMU => 'start'"));
			puts_P(PSTR("MMU <= 'S1'"));
		    mmu_puts_P(PSTR("S1\n")); //send 'read version' request
			mmu_state = -2;
		}
		else if (millis() > 30000) //30sec after reset disable mmu
		{
			puts_P(PSTR("MMU not responding - DISABLED"));
			mmu_state = 0;
		}
		return;
	case -2:
		if (mmu_rx_ok() > 0)
		{
			fscanf_P(uart2io, PSTR("%u"), &mmu_version); //scan version from buffer
			printf_P(PSTR("MMU => '%dok'\n"), mmu_version);
			puts_P(PSTR("MMU <= 'S2'"));
		    mmu_puts_P(PSTR("S2\n")); //send 'read buildnr' request
			mmu_state = -3;
		}
		return;
	case -3:
		if (mmu_rx_ok() > 0)
		{
			fscanf_P(uart2io, PSTR("%u"), &mmu_buildnr); //scan buildnr from buffer
			printf_P(PSTR("MMU => '%dok'\n"), mmu_buildnr);
			puts_P(PSTR("MMU <= 'P0'"));
		    mmu_puts_P(PSTR("P0\n")); //send 'read finda' request
			mmu_state = -4;
		}
		return;
	case -4:
		if (mmu_rx_ok() > 0)
		{
			fscanf_P(uart2io, PSTR("%hhu"), &mmu_finda); //scan finda from buffer
			printf_P(PSTR("MMU => '%dok'\n"), mmu_finda);
			puts_P(PSTR("MMU - ENABLED"));
			mmu_enabled = true;
			mmu_state = 1;
		}
		return;
	}
}

void mmu_reset(void)
{
#ifdef MMU_HWRESET                             //HW - pulse reset pin
	digitalWrite(MMU_RST_PIN, LOW);
	_delay_us(100);
	digitalWrite(MMU_RST_PIN, HIGH);
#else                                          //SW - send X0 command
    mmu_puts_P(PSTR("X0\n"));
#endif
}

int8_t mmu_set_filament_type(uint8_t extruder, uint8_t filament)
{
	printf_P(PSTR("MMU <= 'F%d %d'\n"), extruder, filament);
	mmu_printf_P(PSTR("F%d %d\n"), extruder, filament);
	unsigned char timeout = MMU_TIMEOUT;       //10x100ms
	while ((mmu_rx_ok() <= 0) && (--timeout))
		delay_keep_alive(MMU_TODELAY);
	return timeout?1:0;
}

bool mmu_get_response(bool timeout)
{
	printf_P(PSTR("mmu_get_response - begin\n"));
	//waits for "ok" from mmu
	//function returns true if "ok" was received
	//if timeout is set to true function return false if there is no "ok" received before timeout
	bool response = true;
	LongTimer mmu_get_reponse_timeout;
	KEEPALIVE_STATE(IN_PROCESS);
	mmu_get_reponse_timeout.start();
	while (mmu_rx_ok() <= 0)
	{
		delay_keep_alive(100);
		if (timeout && mmu_get_reponse_timeout.expired(5 * 60 * 1000ul))
		{ //5 minutes timeout
			response = false;
			break;
		}
	}
	printf_P(PSTR("mmu_get_response - end %d\n"), response?1:0);
	return response;
}


void manage_response(bool move_axes, bool turn_off_nozzle)
{
	bool response = false;
	mmu_print_saved = false;
	bool lcd_update_was_enabled = false;
	float hotend_temp_bckp = degTargetHotend(active_extruder);
	float z_position_bckp = current_position[Z_AXIS];
	float x_position_bckp = current_position[X_AXIS];
	float y_position_bckp = current_position[Y_AXIS];	
	while(!response)
	{
		  response = mmu_get_response(true); //wait for "ok" from mmu
		  if (!response) { //no "ok" was received in reserved time frame, user will fix the issue on mmu unit
			  if (!mmu_print_saved) { //first occurence, we are saving current position, park print head in certain position and disable nozzle heater
				  if (lcd_update_enabled) {
					  lcd_update_was_enabled = true;
					  lcd_update_enable(false);
				  }
				  st_synchronize();
				  mmu_print_saved = true;
				  
				  hotend_temp_bckp = degTargetHotend(active_extruder);
				  if (move_axes) {
					  z_position_bckp = current_position[Z_AXIS];
					  x_position_bckp = current_position[X_AXIS];
					  y_position_bckp = current_position[Y_AXIS];
				  
					  //lift z
					  current_position[Z_AXIS] += Z_PAUSE_LIFT;
					  if (current_position[Z_AXIS] > Z_MAX_POS) current_position[Z_AXIS] = Z_MAX_POS;
					  plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 15, active_extruder);
					  st_synchronize();
					  					  
					  //Move XY to side
					  current_position[X_AXIS] = X_PAUSE_POS;
					  current_position[Y_AXIS] = Y_PAUSE_POS;
					  plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 50, active_extruder);
					  st_synchronize();
				  }
				  if (turn_off_nozzle) {
					  //set nozzle target temperature to 0
					  setAllTargetHotends(0);
					  printf_P(PSTR("MMU not responding\n"));
					  lcd_show_fullscreen_message_and_wait_P(_i("MMU needs user attention. Please press knob to resume nozzle target temperature."));
					  setTargetHotend(hotend_temp_bckp, active_extruder);
					  while ((degTargetHotend(active_extruder) - degHotend(active_extruder)) > 5) {
						  delay_keep_alive(1000);
						  lcd_wait_for_heater();
					  }
				  }
			  }
			  lcd_display_message_fullscreen_P(_i("Check MMU. Fix the issue and then press button on MMU unit."));
		  }
		  else if (mmu_print_saved) {
			  printf_P(PSTR("MMU start responding\n"));
			  lcd_clear();
			  lcd_display_message_fullscreen_P(_i("MMU OK. Resuming..."));
			  if (move_axes) {
				  current_position[X_AXIS] = x_position_bckp;
				  current_position[Y_AXIS] = y_position_bckp;
				  plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 50, active_extruder);
				  st_synchronize();
				  current_position[Z_AXIS] = z_position_bckp;
				  plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 15, active_extruder);
				  st_synchronize();
			  }
			  else {
				  delay_keep_alive(1000); //delay just for showing MMU OK message for a while in case that there are no xyz movements
			  }
		  }
	}
	if (lcd_update_was_enabled) lcd_update_enable(true);
}

void mmu_load_to_nozzle()
{
	st_synchronize();
	
	bool saved_e_relative_mode = axis_relative_modes[E_AXIS];
	if (!saved_e_relative_mode) axis_relative_modes[E_AXIS] = true;
	current_position[E_AXIS] += 7.2f;
    float feedrate = 562;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], feedrate / 60, active_extruder);
    st_synchronize();
	current_position[E_AXIS] += 14.4f;
	feedrate = 871;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], feedrate / 60, active_extruder);
    st_synchronize();
	current_position[E_AXIS] += 36.0f;
	feedrate = 1393;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], feedrate / 60, active_extruder);
    st_synchronize();
	current_position[E_AXIS] += 14.4f;
	feedrate = 871;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], feedrate / 60, active_extruder);
    st_synchronize();
	if (!saved_e_relative_mode) axis_relative_modes[E_AXIS] = false;
}

void mmu_M600_load_filament(bool automatic)
{ 
	//load filament for mmu v2

		  bool response = false;
		  bool yes = false;
		  if (!automatic) {
			  yes = lcd_show_fullscreen_message_yes_no_and_wait_P(_i("Do you want to switch extruder?"), false);
			  if(yes) tmp_extruder = choose_extruder_menu();
			  else tmp_extruder = mmu_extruder;

		  }
		  else {
			  tmp_extruder = (tmp_extruder+1)%5;
		  }
		  lcd_update_enable(false);
		  lcd_clear();
		  lcd_set_cursor(0, 1); lcd_puts_P(_T(MSG_LOADING_FILAMENT));
		  lcd_print(" ");
		  lcd_print(tmp_extruder + 1);
		  snmm_filaments_used |= (1 << tmp_extruder); //for stop print
		  printf_P(PSTR("T code: %d \n"), tmp_extruder);
		  mmu_printf_P(PSTR("T%d\n"), tmp_extruder);

		  manage_response(false, true);
    	  mmu_extruder = tmp_extruder; //filament change is finished

		  mmu_load_to_nozzle();

		  st_synchronize();
		  current_position[E_AXIS]+= FILAMENTCHANGE_FINALFEED ;
		  plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 2, active_extruder);
}


void extr_mov(float shift, float feed_rate)
{ //move extruder no matter what the current heater temperature is
	set_extrude_min_temp(.0);
	current_position[E_AXIS] += shift;
	plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], feed_rate, active_extruder);
	set_extrude_min_temp(EXTRUDE_MINTEMP);
}


void change_extr(int extr) { //switches multiplexer for extruders
#ifdef SNMM
	st_synchronize();
	delay(100);

	disable_e0();
	disable_e1();
	disable_e2();

	mmu_extruder = extr;

	pinMode(E_MUX0_PIN, OUTPUT);
	pinMode(E_MUX1_PIN, OUTPUT);

	switch (extr) {
	case 1:
		WRITE(E_MUX0_PIN, HIGH);
		WRITE(E_MUX1_PIN, LOW);
		
		break;
	case 2:
		WRITE(E_MUX0_PIN, LOW);
		WRITE(E_MUX1_PIN, HIGH);
		
		break;
	case 3:
		WRITE(E_MUX0_PIN, HIGH);
		WRITE(E_MUX1_PIN, HIGH);
		
		break;
	default:
		WRITE(E_MUX0_PIN, LOW);
		WRITE(E_MUX1_PIN, LOW);
		
		break;
	}
	delay(100);
#endif
}

int get_ext_nr()
{ //reads multiplexer input pins and return current extruder number (counted from 0)
#ifndef SNMM
	return(mmu_extruder); //update needed
#else 
	return(2 * READ(E_MUX1_PIN) + READ(E_MUX0_PIN));
#endif
}


void display_loading()
{
	switch (mmu_extruder) 
	{
	case 1: lcd_display_message_fullscreen_P(_T(MSG_FILAMENT_LOADING_T1)); break;
	case 2: lcd_display_message_fullscreen_P(_T(MSG_FILAMENT_LOADING_T2)); break;
	case 3: lcd_display_message_fullscreen_P(_T(MSG_FILAMENT_LOADING_T3)); break;
	default: lcd_display_message_fullscreen_P(_T(MSG_FILAMENT_LOADING_T0)); break;
	}
}

void extr_adj(int extruder) //loading filament for SNMM
{
#ifndef SNMM
    printf_P(PSTR("L%d \n"),extruder);
    fprintf_P(uart2io, PSTR("L%d\n"), extruder);
	
	//show which filament is currently loaded
	
	lcd_update_enable(false);
	lcd_clear();
	lcd_set_cursor(0, 1); lcd_puts_P(_T(MSG_LOADING_FILAMENT));
	//if(strlen(_T(MSG_LOADING_FILAMENT))>18) lcd.setCursor(0, 1);
	//else lcd.print(" ");
	lcd_print(" ");
	lcd_print(mmu_extruder + 1);

	// get response
	manage_response(false, false);

	lcd_update_enable(true);
	
	
	//lcd_return_to_status();
#else

	bool correct;
	max_feedrate[E_AXIS] =80;
	//max_feedrate[E_AXIS] = 50;
	START:
	lcd_clear();
	lcd_set_cursor(0, 0); 
	switch (extruder) {
	case 1: lcd_display_message_fullscreen_P(_T(MSG_FILAMENT_LOADING_T1)); break;
	case 2: lcd_display_message_fullscreen_P(_T(MSG_FILAMENT_LOADING_T2)); break;
	case 3: lcd_display_message_fullscreen_P(_T(MSG_FILAMENT_LOADING_T3)); break;
	default: lcd_display_message_fullscreen_P(_T(MSG_FILAMENT_LOADING_T0)); break;   
	}
	KEEPALIVE_STATE(PAUSED_FOR_USER);
	do{
		extr_mov(0.001,1000);
		delay_keep_alive(2);
	} while (!lcd_clicked());
	//delay_keep_alive(500);
	KEEPALIVE_STATE(IN_HANDLER);
	st_synchronize();
	//correct = lcd_show_fullscreen_message_yes_no_and_wait_P(MSG_FIL_LOADED_CHECK, false);
	//if (!correct) goto	START;
	//extr_mov(BOWDEN_LENGTH/2.f, 500); //dividing by 2 is there because of max. extrusion length limitation (x_max + y_max)
	//extr_mov(BOWDEN_LENGTH/2.f, 500);
	extr_mov(bowden_length[extruder], 500);
	lcd_clear();
	lcd_set_cursor(0, 0); lcd_puts_P(_T(MSG_LOADING_FILAMENT));
	if(strlen(_T(MSG_LOADING_FILAMENT))>18) lcd_set_cursor(0, 1);
	else lcd_print(" ");
	lcd_print(mmu_extruder + 1);
	lcd_set_cursor(0, 2); lcd_puts_P(_T(MSG_PLEASE_WAIT));
	st_synchronize();
	max_feedrate[E_AXIS] = 50;
	lcd_update_enable(true);
	lcd_return_to_status();
	lcdDrawUpdate = 2;
#endif
}


void extr_unload()
{ //unload just current filament for multimaterial printers
#ifdef SNMM
	float tmp_motor[3] = DEFAULT_PWM_MOTOR_CURRENT;
	float tmp_motor_loud[3] = DEFAULT_PWM_MOTOR_CURRENT_LOUD;
	uint8_t SilentMode = eeprom_read_byte((uint8_t*)EEPROM_SILENT);
#endif

	if (degHotend0() > EXTRUDE_MINTEMP)
	{
#ifndef SNMM
		st_synchronize();
		
		//show which filament is currently unloaded
		lcd_update_enable(false);
		lcd_clear();
		lcd_set_cursor(0, 1); lcd_puts_P(_T(MSG_UNLOADING_FILAMENT));
		lcd_print(" ");
		lcd_print(mmu_extruder + 1);

		current_position[E_AXIS] -= 80;
		plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 2500 / 60, active_extruder);
		st_synchronize();
		printf_P(PSTR("U0\n"));
		fprintf_P(uart2io, PSTR("U0\n"));

		// get response
		manage_response(false, true);

		lcd_update_enable(true);
#else //SNMM

		lcd_clear();
		lcd_display_message_fullscreen_P(PSTR(""));
		max_feedrate[E_AXIS] = 50;
		lcd_set_cursor(0, 0); lcd_puts_P(_T(MSG_UNLOADING_FILAMENT));
		lcd_print(" ");
		lcd_print(mmu_extruder + 1);
		lcd_set_cursor(0, 2); lcd_puts_P(_T(MSG_PLEASE_WAIT));
		if (current_position[Z_AXIS] < 15) {
			current_position[Z_AXIS] += 15; //lifting in Z direction to make space for extrusion
			plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 25, active_extruder);
		}
		
		current_position[E_AXIS] += 10; //extrusion
		plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 10, active_extruder);
		st_current_set(2, E_MOTOR_HIGH_CURRENT);
		if (current_temperature[0] < 230) { //PLA & all other filaments
			current_position[E_AXIS] += 5.4;
			plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 2800 / 60, active_extruder);
			current_position[E_AXIS] += 3.2;
			plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 3000 / 60, active_extruder);
			current_position[E_AXIS] += 3;
			plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 3400 / 60, active_extruder);
		}
		else { //ABS
			current_position[E_AXIS] += 3.1;
			plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 2000 / 60, active_extruder);
			current_position[E_AXIS] += 3.1;
			plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 2500 / 60, active_extruder);
			current_position[E_AXIS] += 4;
			plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 3000 / 60, active_extruder);
			/*current_position[X_AXIS] += 23; //delay
			plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 600 / 60, active_extruder); //delay
			current_position[X_AXIS] -= 23; //delay
			plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 600 / 60, active_extruder); //delay*/
			delay_keep_alive(4700);
		}
	
		max_feedrate[E_AXIS] = 80;
		current_position[E_AXIS] -= (bowden_length[mmu_extruder] + 60 + FIL_LOAD_LENGTH) / 2;
		plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 500, active_extruder);
		current_position[E_AXIS] -= (bowden_length[mmu_extruder] + 60 + FIL_LOAD_LENGTH) / 2;
		plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 500, active_extruder);
		st_synchronize();
		//st_current_init();
		if (SilentMode != SILENT_MODE_OFF) st_current_set(2, tmp_motor[2]); //set back to normal operation currents
		else st_current_set(2, tmp_motor_loud[2]);
		lcd_update_enable(true);
		lcd_return_to_status();
		max_feedrate[E_AXIS] = 50;
#endif //SNMM
	}
	else
	{
		lcd_clear();
		lcd_set_cursor(0, 0);
		lcd_puts_P(_T(MSG_ERROR));
		lcd_set_cursor(0, 2);
		lcd_puts_P(_T(MSG_PREHEAT_NOZZLE));
		delay(2000);
		lcd_clear();
	}
	//lcd_return_to_status();
}

//wrapper functions for loading filament
void extr_adj_0()
{
#ifndef SNMM
	enquecommand_P(PSTR("M701 E0"));
#else
	change_extr(0);
	extr_adj(0);
#endif
}

void extr_adj_1()
{
#ifndef SNMM
	enquecommand_P(PSTR("M701 E1"));
#else
	change_extr(1);
	extr_adj(1);
#endif
}

void extr_adj_2()
{
#ifndef SNMM
	enquecommand_P(PSTR("M701 E2"));
#else
	change_extr(2);
	extr_adj(2);
#endif
}

void extr_adj_3()
{
#ifndef SNMM
	enquecommand_P(PSTR("M701 E3"));
#else
	change_extr(3);
	extr_adj(3);
#endif
}

void extr_adj_4()
{
#ifndef SNMM
	enquecommand_P(PSTR("M701 E4"));
#else
	change_extr(4);
	extr_adj(4);
#endif
}

void load_all()
{
#ifndef SNMM
	enquecommand_P(PSTR("M701 E0"));
	enquecommand_P(PSTR("M701 E1"));
	enquecommand_P(PSTR("M701 E2"));
	enquecommand_P(PSTR("M701 E3"));
	enquecommand_P(PSTR("M701 E4"));
#else
	for (int i = 0; i < 4; i++)
	{
		change_extr(i);
		extr_adj(i);
	}
#endif
}

//wrapper functions for changing extruders
void extr_change_0()
{
	change_extr(0);
	lcd_return_to_status();
}

void extr_change_1()
{
	change_extr(1);
	lcd_return_to_status();
}

void extr_change_2()
{
	change_extr(2);
	lcd_return_to_status();
}

void extr_change_3()
{
	change_extr(3);
	lcd_return_to_status();
}

//wrapper functions for unloading filament
void extr_unload_all()
{
	if (degHotend0() > EXTRUDE_MINTEMP)
	{
		for (int i = 0; i < 4; i++)
		{
			change_extr(i);
			extr_unload();
		}
	}
	else
	{
		lcd_clear();
		lcd_set_cursor(0, 0);
		lcd_puts_P(_T(MSG_ERROR));
		lcd_set_cursor(0, 2);
		lcd_puts_P(_T(MSG_PREHEAT_NOZZLE));
		delay(2000);
		lcd_clear();
		lcd_return_to_status();
	}
}

//unloading just used filament (for snmm)
void extr_unload_used()
{
	if (degHotend0() > EXTRUDE_MINTEMP) {
		for (int i = 0; i < 4; i++) {
			if (snmm_filaments_used & (1 << i)) {
				change_extr(i);
				extr_unload();
			}
		}
		snmm_filaments_used = 0;
	}
	else {
		lcd_clear();
		lcd_set_cursor(0, 0);
		lcd_puts_P(_T(MSG_ERROR));
		lcd_set_cursor(0, 2);
		lcd_puts_P(_T(MSG_PREHEAT_NOZZLE));
		delay(2000);
		lcd_clear();
		lcd_return_to_status();
	}
}

void extr_unload_0()
{
	change_extr(0);
	extr_unload();
}

void extr_unload_1()
{
	change_extr(1);
	extr_unload();
}

void extr_unload_2()
{
	change_extr(2);
	extr_unload();
}

void extr_unload_3()
{
	change_extr(3);
	extr_unload();
}

void extr_unload_4()
{
	change_extr(4);
	extr_unload();
}
