#include "Marlin.h"

#include "fsensor.h"
#include <avr/pgmspace.h>
#include "pat9125.h"
#include "stepper.h"
#include "planner.h"
#include "fastio.h"
#include "cmdqueue.h"

//Basic params
#define FSENSOR_CHUNK_LEN    0.64F  //filament sensor chunk length 0.64mm
#define FSENSOR_ERR_MAX         17  //filament sensor maximum error count for runout detection

//Optical quality meassurement params
#define FSENSOR_OQ_MAX_ES      6    //maximum error sum while loading (length ~64mm = 100chunks)
#define FSENSOR_OQ_MAX_EM      2    //maximum error counter value while loading
#define FSENSOR_OQ_MIN_YD      2    //minimum yd per chunk (applied to avg value)
#define FSENSOR_OQ_MAX_YD      200  //maximum yd per chunk (applied to avg value)
#define FSENSOR_OQ_MAX_PD      4    //maximum positive deviation (= yd_max/yd_avg)
#define FSENSOR_OQ_MAX_ND      5    //maximum negative deviation (= yd_avg/yd_min)
#define FSENSOR_OQ_MAX_SH      13   //maximum shutter value


const char ERRMSG_PAT9125_NOT_RESP[] PROGMEM = "PAT9125 not responding (%d)!\n";

#define FSENSOR_INT_PIN         63  //filament sensor interrupt pin PK1
#define FSENSOR_INT_PIN_MSK   0x02  //filament sensor interrupt pin mask (bit1)

extern void stop_and_save_print_to_ram(float z_move, float e_move);
extern void restore_print_from_ram_and_continue(float e_move);
extern int8_t FSensorStateMenu;

void fsensor_stop_and_save_print(void)
{
	printf_P(PSTR("fsensor_stop_and_save_print\n"));
	stop_and_save_print_to_ram(0, 0); //XYZE - no change	
}

void fsensor_restore_print_and_continue(void)
{
	printf_P(PSTR("fsensor_restore_print_and_continue\n"));
	restore_print_from_ram_and_continue(0); //XYZ = orig, E - no change
}

//uint8_t fsensor_int_pin = FSENSOR_INT_PIN;
uint8_t fsensor_int_pin_old = 0;
int16_t fsensor_chunk_len = 0;

//enabled = initialized and sampled every chunk event
bool fsensor_enabled = true;
//runout watching is done in fsensor_update (called from main loop)
bool fsensor_watch_runout = true;
//not responding - is set if any communication error occured durring initialization or readout
bool fsensor_not_responding = false;
//printing saved
bool fsensor_printing_saved = false;

//number of errors, updated in ISR
uint8_t fsensor_err_cnt = 0;
//variable for accumolating step count (updated callbacks from stepper and ISR)
int16_t fsensor_st_cnt = 0;
//last dy value from pat9125 sensor (used in ISR)
int16_t fsensor_dy_old = 0;

//log flag: 0=log disabled, 1=log enabled
uint8_t fsensor_log = 1;

////////////////////////////////////////////////////////////////////////////////
//filament autoload variables

//autoload feature enabled
bool fsensor_autoload_enabled = true;

//autoload watching enable/disable flag
bool fsensor_watch_autoload = false;
//
uint16_t fsensor_autoload_y;
//
uint8_t fsensor_autoload_c;
//
uint32_t fsensor_autoload_last_millis;
//
uint8_t fsensor_autoload_sum;

////////////////////////////////////////////////////////////////////////////////
//filament optical quality meassurement variables

//meassurement enable/disable flag
bool fsensor_oq_meassure = false;
//skip-chunk counter, for accurate meassurement is necesary to skip first chunk...
uint8_t  fsensor_oq_skipchunk;
//number of samples from start of meassurement
uint8_t fsensor_oq_samples;
//sum of steps in positive direction movements
uint16_t fsensor_oq_st_sum;
//sum of deltas in positive direction movements
uint16_t fsensor_oq_yd_sum;
//sum of errors durring meassurement
uint16_t fsensor_oq_er_sum;
//max error counter value durring meassurement
uint8_t  fsensor_oq_er_max;
//minimum delta value
uint16_t fsensor_oq_yd_min;
//maximum delta value
uint16_t fsensor_oq_yd_max;
//sum of shutter value
uint16_t fsensor_oq_sh_sum;


void fsensor_init(void)
{
	uint8_t pat9125 = pat9125_init();
    printf_P(PSTR("PAT9125_init:%hhu\n"), pat9125);
	uint8_t fsensor = eeprom_read_byte((uint8_t*)EEPROM_FSENSOR);
	fsensor_autoload_enabled=eeprom_read_byte((uint8_t*)EEPROM_FSENS_AUTOLOAD_ENABLED);
	fsensor_chunk_len = (int16_t)(FSENSOR_CHUNK_LEN * axis_steps_per_unit[E_AXIS]);

	if (!pat9125)
	{
		fsensor = 0; //disable sensor
		fsensor_not_responding = true;
	}
	else
		fsensor_not_responding = false;
	if (fsensor)
		fsensor_enable();
	else
		fsensor_disable();
	printf_P(PSTR("FSensor %S\n"), (fsensor_enabled?PSTR("ENABLED"):PSTR("DISABLED\n")));
}

bool fsensor_enable(void)
{
	uint8_t pat9125 = pat9125_init();
    printf_P(PSTR("PAT9125_init:%hhu\n"), pat9125);
	if (pat9125)
		fsensor_not_responding = false;
	else
		fsensor_not_responding = true;
	fsensor_enabled = pat9125?true:false;
	fsensor_watch_runout = true;
	fsensor_oq_meassure = false;
	fsensor_err_cnt = 0;
	fsensor_dy_old = 0;
	eeprom_update_byte((uint8_t*)EEPROM_FSENSOR, fsensor_enabled?0x01:0x00); 
	FSensorStateMenu = fsensor_enabled?1:0;


	return fsensor_enabled;
}

void fsensor_disable(void)
{
	fsensor_enabled = false;
	eeprom_update_byte((uint8_t*)EEPROM_FSENSOR, 0x00); 
	FSensorStateMenu = 0;
}

void fsensor_autoload_set(bool State)
{
	fsensor_autoload_enabled = State;
	eeprom_update_byte((unsigned char *)EEPROM_FSENS_AUTOLOAD_ENABLED, fsensor_autoload_enabled);
}

void pciSetup(byte pin)
{
	*digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin)); // enable pin
	PCIFR |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
	PCICR |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group 
}

void fsensor_autoload_check_start(void)
{
//	puts_P(_N("fsensor_autoload_check_start\n"));
	if (!fsensor_enabled) return;
	if (!fsensor_autoload_enabled) return;
	if (fsensor_watch_autoload) return;
	if (!pat9125_update_y()) //update sensor
	{
		fsensor_disable();
		fsensor_not_responding = true;
		fsensor_watch_autoload = false;
		printf_P(ERRMSG_PAT9125_NOT_RESP, 3);
		return;
	}
	puts_P(_N("fsensor_autoload_check_start - autoload ENABLED\n"));
	fsensor_autoload_y = pat9125_y; //save current y value
	fsensor_autoload_c = 0; //reset number of changes counter
	fsensor_autoload_sum = 0;
	fsensor_autoload_last_millis = millis();
	fsensor_watch_runout = false;
	fsensor_watch_autoload = true;
	fsensor_err_cnt = 0;
}

void fsensor_autoload_check_stop(void)
{
//	puts_P(_N("fsensor_autoload_check_stop\n"));
	if (!fsensor_enabled) return;
//	puts_P(_N("fsensor_autoload_check_stop 1\n"));
	if (!fsensor_autoload_enabled) return;
//	puts_P(_N("fsensor_autoload_check_stop 2\n"));
	if (!fsensor_watch_autoload) return;
	puts_P(_N("fsensor_autoload_check_stop - autoload DISABLED\n"));
	fsensor_autoload_sum = 0;
	fsensor_watch_autoload = false;
	fsensor_watch_runout = true;
	fsensor_err_cnt = 0;
}

bool fsensor_check_autoload(void)
{
	if (!fsensor_enabled) return false;
	if (!fsensor_autoload_enabled) return false;
	if (!fsensor_watch_autoload)
	{
		fsensor_autoload_check_start();
		return false;
	}
	uint8_t fsensor_autoload_c_old = fsensor_autoload_c;
	if ((millis() - fsensor_autoload_last_millis) < 25) return false;
	fsensor_autoload_last_millis = millis();
	if (!pat9125_update_y()) //update sensor
	{
		fsensor_disable();
		fsensor_not_responding = true;
		printf_P(ERRMSG_PAT9125_NOT_RESP, 2);
		return false;
	}
	int16_t dy = pat9125_y - fsensor_autoload_y;
	if (dy) //? dy value is nonzero
	{
		if (dy > 0) //? delta-y value is positive (inserting)
		{
			fsensor_autoload_sum += dy;
			fsensor_autoload_c += 3; //increment change counter by 3
		}
		else if (fsensor_autoload_c > 1)
			fsensor_autoload_c -= 2; //decrement change counter by 2 
		fsensor_autoload_y = pat9125_y; //save current value
	}
	else if (fsensor_autoload_c > 0)
		fsensor_autoload_c--;
	if (fsensor_autoload_c == 0) fsensor_autoload_sum = 0;
//	puts_P(_N("fsensor_check_autoload\n"));
//	if (fsensor_autoload_c != fsensor_autoload_c_old)
//		printf_P(PSTR("fsensor_check_autoload dy=%d c=%d sum=%d\n"), dy, fsensor_autoload_c, fsensor_autoload_sum);
//	if ((fsensor_autoload_c >= 15) && (fsensor_autoload_sum > 30))
	if ((fsensor_autoload_c >= 12) && (fsensor_autoload_sum > 20))
	{
//		puts_P(_N("fsensor_check_autoload = true !!!\n"));
		return true;
	}
	return false;
}

void fsensor_oq_meassure_start(uint8_t skip)
{
	if (!fsensor_enabled) return;
	printf_P(PSTR("fsensor_oq_meassure_start\n"));
	fsensor_oq_skipchunk = skip;
	fsensor_oq_samples = 0;
	fsensor_oq_st_sum = 0;
	fsensor_oq_yd_sum = 0;
	fsensor_oq_er_sum = 0;
	fsensor_oq_er_max = 0;
	fsensor_oq_yd_min = FSENSOR_OQ_MAX_YD;
	fsensor_oq_yd_max = 0;
	fsensor_oq_sh_sum = 0;
	pat9125_update();
	pat9125_y = 0;
	fsensor_watch_runout = false;
	fsensor_oq_meassure = true;
}

void fsensor_oq_meassure_stop(void)
{
	if (!fsensor_enabled) return;
	printf_P(PSTR("fsensor_oq_meassure_stop, %hhu samples\n"), fsensor_oq_samples);
	printf_P(_N(" st_sum=%u yd_sum=%u er_sum=%u er_max=%hhu\n"), fsensor_oq_st_sum, fsensor_oq_yd_sum, fsensor_oq_er_sum, fsensor_oq_er_max);
	printf_P(_N(" yd_min=%u yd_max=%u yd_avg=%u sh_avg=%u\n"), fsensor_oq_yd_min, fsensor_oq_yd_max, (uint16_t)((uint32_t)fsensor_oq_yd_sum * fsensor_chunk_len / fsensor_oq_st_sum), (uint16_t)(fsensor_oq_sh_sum / fsensor_oq_samples));
	fsensor_oq_meassure = false;
	fsensor_watch_runout = true;
	fsensor_err_cnt = 0;
}

const char _OK[] PROGMEM = "OK";
const char _NG[] PROGMEM = "NG!";

bool fsensor_oq_result(void)
{
	if (!fsensor_enabled) return true;
	printf_P(_N("fsensor_oq_result\n"));
	bool res_er_sum = (fsensor_oq_er_sum <= FSENSOR_OQ_MAX_ES);
	printf_P(_N(" er_sum = %u %S\n"), fsensor_oq_er_sum, (res_er_sum?_OK:_NG));
	bool res_er_max = (fsensor_oq_er_max <= FSENSOR_OQ_MAX_EM);
	printf_P(_N(" er_max = %hhu %S\n"), fsensor_oq_er_max, (res_er_max?_OK:_NG));
	uint8_t yd_avg = ((uint32_t)fsensor_oq_yd_sum * fsensor_chunk_len / fsensor_oq_st_sum);
	bool res_yd_avg = (yd_avg >= FSENSOR_OQ_MIN_YD) && (yd_avg <= FSENSOR_OQ_MAX_YD);
	printf_P(_N(" yd_avg = %hhu %S\n"), yd_avg, (res_yd_avg?_OK:_NG));
	bool res_yd_max = (fsensor_oq_yd_max <= (yd_avg * FSENSOR_OQ_MAX_PD));
	printf_P(_N(" yd_max = %u %S\n"), fsensor_oq_yd_max, (res_yd_max?_OK:_NG));
	bool res_yd_min = (fsensor_oq_yd_min >= (yd_avg / FSENSOR_OQ_MAX_ND));
	printf_P(_N(" yd_min = %u %S\n"), fsensor_oq_yd_min, (res_yd_min?_OK:_NG));

	uint16_t yd_dev = (fsensor_oq_yd_max - yd_avg) + (yd_avg - fsensor_oq_yd_min);
	uint16_t yd_qua = 10 * yd_avg / (yd_dev + 1);
	printf_P(_N(" yd_dev = %u\n"), yd_dev);
	printf_P(_N(" yd_qua = %u\n"), yd_qua);

	uint8_t sh_avg = (fsensor_oq_sh_sum / fsensor_oq_samples);
	bool res_sh_avg = (sh_avg <= FSENSOR_OQ_MAX_SH);
	if (yd_qua >= 8) res_sh_avg = true;

	printf_P(_N(" sh_avg = %hhu %S\n"), sh_avg, (res_sh_avg?_OK:_NG));
	bool res = res_er_sum && res_er_max && res_yd_avg && res_yd_max && res_yd_min && res_sh_avg;
	printf_P(_N("fsensor_oq_result %S\n"), (res?_OK:_NG));
	return res;
}

ISR(PCINT2_vect)
{
	if (!((fsensor_int_pin_old ^ PINK) & FSENSOR_INT_PIN_MSK)) return;
	fsensor_int_pin_old = PINK;
	static bool _lock = false;
	if (_lock) return;
	_lock = true;
	int st_cnt = fsensor_st_cnt;
	fsensor_st_cnt = 0;
	sei();
	uint8_t old_err_cnt = fsensor_err_cnt;
	uint8_t pat9125_res = fsensor_oq_meassure?pat9125_update():pat9125_update_y();
	if (!pat9125_res)
	{
		fsensor_disable();
		fsensor_not_responding = true;
		printf_P(ERRMSG_PAT9125_NOT_RESP, 1);
	}
	if (st_cnt != 0)
	{ //movement
		if (st_cnt > 0) //positive movement
		{
			if (pat9125_y < 0)
			{
				if (fsensor_err_cnt)
					fsensor_err_cnt += 2;
				else
					fsensor_err_cnt++;
			}
			else if (pat9125_y > 0)
			{
				if (fsensor_err_cnt)
					fsensor_err_cnt--;
			}
			else //(pat9125_y == 0)
				if (((fsensor_dy_old <= 0) || (fsensor_err_cnt)) && (st_cnt > (fsensor_chunk_len >> 1)))
					fsensor_err_cnt++;
			if (fsensor_oq_meassure)
			{
				if (fsensor_oq_skipchunk)
				{
					fsensor_oq_skipchunk--;
					fsensor_err_cnt = 0;
				}
				else
				{
					if (st_cnt == fsensor_chunk_len)
					{
						if (pat9125_y > 0) if (fsensor_oq_yd_min > pat9125_y) fsensor_oq_yd_min = (fsensor_oq_yd_min + pat9125_y) / 2;
						if (pat9125_y >= 0) if (fsensor_oq_yd_max < pat9125_y) fsensor_oq_yd_max = (fsensor_oq_yd_max + pat9125_y) / 2;
					}
					fsensor_oq_samples++;
					fsensor_oq_st_sum += st_cnt;
					if (pat9125_y > 0) fsensor_oq_yd_sum += pat9125_y;
					if (fsensor_err_cnt > old_err_cnt)
						fsensor_oq_er_sum += (fsensor_err_cnt - old_err_cnt);
					if (fsensor_oq_er_max < fsensor_err_cnt)
						fsensor_oq_er_max = fsensor_err_cnt;
					fsensor_oq_sh_sum += pat9125_s;
				}
			}
		}
		else //negative movement
		{
		}
	}
	else
	{ //no movement
	}

#ifdef DEBUG_FSENSOR_LOG
	if (fsensor_log)
	{
		printf_P(_N("FSENSOR cnt=%d dy=%d err=%hhu %S\n"), st_cnt, pat9125_y, fsensor_err_cnt, (fsensor_err_cnt > old_err_cnt)?_N("NG!"):_N("OK"));
		if (fsensor_oq_meassure) printf_P(_N("FSENSOR st_sum=%u yd_sum=%u er_sum=%u er_max=%hhu yd_max=%u\n"), fsensor_oq_st_sum, fsensor_oq_yd_sum, fsensor_oq_er_sum, fsensor_oq_er_max, fsensor_oq_yd_max);
	}
#endif //DEBUG_FSENSOR_LOG

	fsensor_dy_old = pat9125_y;
	pat9125_y = 0;

	_lock = false;
	return;
}

void fsensor_st_block_begin(block_t* bl)
{
	if (!fsensor_enabled) return;
	if (((fsensor_st_cnt > 0) && (bl->direction_bits & 0x8)) || 
		((fsensor_st_cnt < 0) && !(bl->direction_bits & 0x8)))
	{
		if (_READ(63)) _WRITE(63, LOW);
		else _WRITE(63, HIGH);
	}
}

void fsensor_st_block_chunk(block_t* bl, int cnt)
{
	if (!fsensor_enabled) return;
	fsensor_st_cnt += (bl->direction_bits & 0x8)?-cnt:cnt;
	if ((fsensor_st_cnt >= fsensor_chunk_len) || (fsensor_st_cnt <= -fsensor_chunk_len))
	{
		if (_READ(63)) _WRITE(63, LOW);
		else _WRITE(63, HIGH);
	}
}

void fsensor_update(void)
{
	if (fsensor_enabled)
	{
		if (fsensor_printing_saved)
		{
			fsensor_restore_print_and_continue();
			fsensor_printing_saved = false;
			fsensor_watch_runout = true;
			fsensor_err_cnt = 0;
		}
		else if (fsensor_watch_runout && (fsensor_err_cnt > FSENSOR_ERR_MAX))
		{
			bool autoload_enabled_tmp = fsensor_autoload_enabled;
			fsensor_autoload_enabled = false;

			fsensor_stop_and_save_print();
			fsensor_printing_saved = true;

			fsensor_err_cnt = 0;
			fsensor_oq_meassure_start(0);

//			st_synchronize();
//			for (int axis = X_AXIS; axis <= E_AXIS; axis++)
//				current_position[axis] = st_get_position_mm(axis);
/*
			current_position[E_AXIS] -= 3;
			plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 200 / 60, active_extruder);
			st_synchronize();

			current_position[E_AXIS] += 3;
			plan_buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 200 / 60, active_extruder);
			st_synchronize();
*/

			enquecommand_front_P((PSTR("G1 E-3 F200")));
			process_commands();
			cmdqueue_pop_front();
			st_synchronize();

			enquecommand_front_P((PSTR("G1 E3 F200")));
			process_commands();
			cmdqueue_pop_front();
			st_synchronize();

			fsensor_oq_meassure_stop();

			bool err = false;
			err |= (fsensor_oq_er_sum > 1);
			err |= (fsensor_oq_yd_sum < (4 * FSENSOR_OQ_MIN_YD));
			if (!err)
			{
				printf_P(PSTR("fsensor_err_cnt = 0\n"));
				fsensor_restore_print_and_continue();
				fsensor_printing_saved = false;
			}
			else
			{
				printf_P(PSTR("fsensor_update - M600\n"));
				eeprom_update_byte((uint8_t*)EEPROM_FERROR_COUNT, eeprom_read_byte((uint8_t*)EEPROM_FERROR_COUNT) + 1);
				eeprom_update_word((uint16_t*)EEPROM_FERROR_COUNT_TOT, eeprom_read_word((uint16_t*)EEPROM_FERROR_COUNT_TOT) + 1);
				enquecommand_front_P((PSTR("M600")));
				fsensor_watch_runout = false;
			}
			fsensor_autoload_enabled = autoload_enabled_tmp;
		}
	}
}

void fsensor_setup_interrupt(void)
{

	pinMode(FSENSOR_INT_PIN, OUTPUT);
	digitalWrite(FSENSOR_INT_PIN, LOW);
	fsensor_int_pin_old = 0;

	pciSetup(FSENSOR_INT_PIN);
}
