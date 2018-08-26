// Tonokip RepRap firmware rewrite based off of Hydra-mmm firmware.
// License: GPL

#ifndef MARLIN_H
#define MARLIN_H

#define  FORCE_INLINE __attribute__((always_inline)) inline

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>


#include "fastio.h"
#include "Configuration.h"
#include "pins.h"

#ifndef AT90USB
#define  HardwareSerial_h // trick to disable the standard HWserial
#endif

#if (ARDUINO >= 100)
# include "Arduino.h"
#else
# include "WProgram.h"
#endif

// Arduino < 1.0.0 does not define this, so we need to do it ourselves
#ifndef analogInputToDigitalPin
# define analogInputToDigitalPin(p) ((p) + A0)
#endif

#ifdef AT90USB
#include "HardwareSerial.h"
#endif

#include "MarlinSerial.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//#include "WString.h"

#ifdef AT90USB
   #ifdef BTENABLED
         #define MYSERIAL bt
   #else
         #define MYSERIAL Serial
   #endif // BTENABLED
#else
  #define MYSERIAL MSerial
#endif

#include "lcd.h"

extern FILE _uartout;
#define uartout (&_uartout)

#define SERIAL_PROTOCOL(x) (MYSERIAL.print(x))
#define SERIAL_PROTOCOL_F(x,y) (MYSERIAL.print(x,y))
#define SERIAL_PROTOCOLPGM(x) (serialprintPGM(PSTR(x)))
#define SERIAL_PROTOCOLRPGM(x) (serialprintPGM((x)))
#define SERIAL_PROTOCOLLN(x) (MYSERIAL.print(x),MYSERIAL.write('\n'))
#define SERIAL_PROTOCOLLNPGM(x) (serialprintPGM(PSTR(x)),MYSERIAL.write('\n'))
#define SERIAL_PROTOCOLLNRPGM(x) (serialprintPGM((x)),MYSERIAL.write('\n'))


extern const char errormagic[] PROGMEM;
extern const char echomagic[] PROGMEM;

#define SERIAL_ERROR_START (serialprintPGM(errormagic))
#define SERIAL_ERROR(x) SERIAL_PROTOCOL(x)
#define SERIAL_ERRORPGM(x) SERIAL_PROTOCOLPGM(x)
#define SERIAL_ERRORRPGM(x) SERIAL_PROTOCOLRPGM(x)
#define SERIAL_ERRORLN(x) SERIAL_PROTOCOLLN(x)
#define SERIAL_ERRORLNPGM(x) SERIAL_PROTOCOLLNPGM(x)
#define SERIAL_ERRORLNRPGM(x) SERIAL_PROTOCOLLNRPGM(x)

#define SERIAL_ECHO_START (serialprintPGM(echomagic))
#define SERIAL_ECHO(x) SERIAL_PROTOCOL(x)
#define SERIAL_ECHOPGM(x) SERIAL_PROTOCOLPGM(x)
#define SERIAL_ECHORPGM(x) SERIAL_PROTOCOLRPGM(x)
#define SERIAL_ECHOLN(x) SERIAL_PROTOCOLLN(x)
#define SERIAL_ECHOLNPGM(x) SERIAL_PROTOCOLLNPGM(x)
#define SERIAL_ECHOLNRPGM(x) SERIAL_PROTOCOLLNRPGM(x)

#define SERIAL_ECHOPAIR(name,value) (serial_echopair_P(PSTR(name),(value)))

void serial_echopair_P(const char *s_P, float v);
void serial_echopair_P(const char *s_P, double v);
void serial_echopair_P(const char *s_P, unsigned long v);


//Things to write to serial from Program memory. Saves 400 to 2k of RAM.
FORCE_INLINE void serialprintPGM(const char *str)
{
  char ch=pgm_read_byte(str);
  while(ch)
  {
    MYSERIAL.write(ch);
    ch=pgm_read_byte(++str);
  }
}

bool is_buffer_empty();
void get_command();
void process_commands();
void ramming();

void manage_inactivity(bool ignore_stepper_queue=false);

#if defined(X_ENABLE_PIN) && X_ENABLE_PIN > -1
  #define  enable_x() WRITE(X_ENABLE_PIN, X_ENABLE_ON)
  #define disable_x() { WRITE(X_ENABLE_PIN,!X_ENABLE_ON); axis_known_position[X_AXIS] = false; }
#else
  #define enable_x() ;
  #define disable_x() ;
#endif

#if defined(Y_ENABLE_PIN) && Y_ENABLE_PIN > -1
  #ifdef Y_DUAL_STEPPER_DRIVERS
    #define  enable_y() { WRITE(Y_ENABLE_PIN, Y_ENABLE_ON); WRITE(Y2_ENABLE_PIN,  Y_ENABLE_ON); }
    #define disable_y() { WRITE(Y_ENABLE_PIN,!Y_ENABLE_ON); WRITE(Y2_ENABLE_PIN, !Y_ENABLE_ON); axis_known_position[Y_AXIS] = false; }
  #else
    #define  enable_y() WRITE(Y_ENABLE_PIN, Y_ENABLE_ON)
    #define disable_y() { WRITE(Y_ENABLE_PIN,!Y_ENABLE_ON); axis_known_position[Y_AXIS] = false; }
  #endif
#else
  #define enable_y() ;
  #define disable_y() ;
#endif

#if defined(Z_ENABLE_PIN) && Z_ENABLE_PIN > -1 
	#if defined(Z_AXIS_ALWAYS_ON)
		  #ifdef Z_DUAL_STEPPER_DRIVERS
			#define  enable_z() { WRITE(Z_ENABLE_PIN, Z_ENABLE_ON); WRITE(Z2_ENABLE_PIN, Z_ENABLE_ON); }
			#define disable_z() { WRITE(Z_ENABLE_PIN,!Z_ENABLE_ON); WRITE(Z2_ENABLE_PIN,!Z_ENABLE_ON); axis_known_position[Z_AXIS] = false; }
		  #else
			#define  enable_z() WRITE(Z_ENABLE_PIN, Z_ENABLE_ON)
			#define  disable_z() ;
		  #endif
	#else
		#ifdef Z_DUAL_STEPPER_DRIVERS
			#define  enable_z() { WRITE(Z_ENABLE_PIN, Z_ENABLE_ON); WRITE(Z2_ENABLE_PIN, Z_ENABLE_ON); }
			#define disable_z() { WRITE(Z_ENABLE_PIN,!Z_ENABLE_ON); WRITE(Z2_ENABLE_PIN,!Z_ENABLE_ON); axis_known_position[Z_AXIS] = false; }
		#else
			#define  enable_z() WRITE(Z_ENABLE_PIN, Z_ENABLE_ON)
			#define disable_z() { WRITE(Z_ENABLE_PIN,!Z_ENABLE_ON); axis_known_position[Z_AXIS] = false; }
		#endif
	#endif
#else
  #define enable_z() ;
  #define disable_z() ;
#endif

//#if defined(Z_ENABLE_PIN) && Z_ENABLE_PIN > -1
//#ifdef Z_DUAL_STEPPER_DRIVERS
//#define  enable_z() { WRITE(Z_ENABLE_PIN, Z_ENABLE_ON); WRITE(Z2_ENABLE_PIN, Z_ENABLE_ON); }
//#define disable_z() { WRITE(Z_ENABLE_PIN,!Z_ENABLE_ON); WRITE(Z2_ENABLE_PIN,!Z_ENABLE_ON); axis_known_position[Z_AXIS] = false; }
//#else
//#define  enable_z() WRITE(Z_ENABLE_PIN, Z_ENABLE_ON)
//#define disable_z() { WRITE(Z_ENABLE_PIN,!Z_ENABLE_ON); axis_known_position[Z_AXIS] = false; }
//#endif
//#else
//#define enable_z() ;
//#define disable_z() ;
//#endif

#if defined(E0_ENABLE_PIN) && (E0_ENABLE_PIN > -1)
  #define enable_e0() WRITE(E0_ENABLE_PIN, E_ENABLE_ON)
  #define disable_e0() WRITE(E0_ENABLE_PIN,!E_ENABLE_ON)
#else
  #define enable_e0()  /* nothing */
  #define disable_e0() /* nothing */
#endif

#if (EXTRUDERS > 1) && defined(E1_ENABLE_PIN) && (E1_ENABLE_PIN > -1)
  #define enable_e1() WRITE(E1_ENABLE_PIN, E_ENABLE_ON)
  #define disable_e1() WRITE(E1_ENABLE_PIN,!E_ENABLE_ON)
#else
  #define enable_e1()  /* nothing */
  #define disable_e1() /* nothing */
#endif

#if (EXTRUDERS > 2) && defined(E2_ENABLE_PIN) && (E2_ENABLE_PIN > -1)
  #define enable_e2() WRITE(E2_ENABLE_PIN, E_ENABLE_ON)
  #define disable_e2() WRITE(E2_ENABLE_PIN,!E_ENABLE_ON)
#else
  #define enable_e2()  /* nothing */
  #define disable_e2() /* nothing */
#endif

enum AxisEnum {X_AXIS=0, Y_AXIS=1, Z_AXIS=2, E_AXIS=3, X_HEAD=4, Y_HEAD=5};
#define X_AXIS_MASK  1
#define Y_AXIS_MASK  2
#define Z_AXIS_MASK  4
#define E_AXIS_MASK  8
#define X_HEAD_MASK 16
#define Y_HEAD_MASK 32

void FlushSerialRequestResend();
void ClearToSend();
void update_currents();

void get_coordinates();
void prepare_move();
void kill(const char *full_screen_message = NULL, unsigned char id = 0);
void Stop();

bool IsStopped();

//put an ASCII command at the end of the current buffer.
void enquecommand(const char *cmd, bool from_progmem = false);

//put an ASCII command at the end of the current buffer, read from flash
#define enquecommand_P(cmd) enquecommand(cmd, true)

//put an ASCII command at the begin of the current buffer
void enquecommand_front(const char *cmd, bool from_progmem = false);

//put an ASCII command at the begin of the current buffer, read from flash
#define enquecommand_front_P(cmd) enquecommand_front(cmd, true)

void repeatcommand_front();

// Remove all lines from the command queue.
void cmdqueue_reset();

void prepare_arc_move(char isclockwise);
void clamp_to_software_endstops(float target[3]);
void refresh_cmd_timeout(void);

// Timer counter, incremented by the 1ms Arduino timer.
// The standard Arduino timer() function returns this value atomically
// by disabling / enabling interrupts. This is costly, if the interrupts are known
// to be disabled.
extern volatile unsigned long timer0_millis;
// An unsynchronized equivalent to a standard Arduino millis() function.
// To be used inside an interrupt routine.
FORCE_INLINE unsigned long millis_nc() { return timer0_millis; }

#ifdef FAST_PWM_FAN
void setPwmFrequency(uint8_t pin, int val);
#endif

#ifndef CRITICAL_SECTION_START
  #define CRITICAL_SECTION_START  unsigned char _sreg = SREG; cli();
  #define CRITICAL_SECTION_END    SREG = _sreg;
#endif //CRITICAL_SECTION_START

extern float homing_feedrate[];
extern bool axis_relative_modes[];
extern int feedmultiply;
extern int extrudemultiply; // Sets extrude multiply factor (in percent) for all extruders
extern bool volumetric_enabled;
extern int extruder_multiply[EXTRUDERS]; // sets extrude multiply factor (in percent) for each extruder individually
extern float filament_size[EXTRUDERS]; // cross-sectional area of filament (in millimeters), typically around 1.75 or 2.85, 0 disables the volumetric calculations for the extruder.
extern float volumetric_multiplier[EXTRUDERS]; // reciprocal of cross-sectional area of filament (in square millimeters), stored this way to reduce computational burden in planner
extern float current_position[NUM_AXIS] ;
extern float destination[NUM_AXIS] ;
extern float add_homing[3];
extern float min_pos[3];
extern float max_pos[3];
extern bool axis_known_position[3];
extern float zprobe_zoffset;
extern int fanSpeed;
extern void homeaxis(int axis, uint8_t cnt = 1, uint8_t* pstep = 0);

#ifdef FAN_SOFT_PWM
extern unsigned char fanSpeedSoftPwm;
#endif

#ifdef FWRETRACT
extern bool autoretract_enabled;
extern bool retracted[EXTRUDERS];
extern float retract_length, retract_length_swap, retract_feedrate, retract_zlift;
extern float retract_recover_length, retract_recover_length_swap, retract_recover_feedrate;
#endif

#ifdef HOST_KEEPALIVE_FEATURE
extern uint8_t host_keepalive_interval;
#endif

extern unsigned long starttime;
extern unsigned long stoptime;
extern int bowden_length[4];
extern bool is_usb_printing;
extern bool homing_flag;
extern bool temp_cal_active;
extern bool loading_flag;
extern unsigned int usb_printing_counter;

extern unsigned long kicktime;

extern unsigned long total_filament_used;
void save_statistics(unsigned long _total_filament_used, unsigned long _total_print_time);
extern unsigned int heating_status;
extern unsigned int status_number;
extern unsigned int heating_status_counter;
extern bool custom_message;
extern unsigned int custom_message_type;
extern unsigned int custom_message_state;
extern char snmm_filaments_used;
extern unsigned long PingTime;
extern unsigned long NcTime;
extern bool no_response;
extern uint8_t important_status;
extern uint8_t saved_filament_type;

extern bool fan_state[2];
extern int fan_edge_counter[2];
extern int fan_speed[2];

// Handling multiple extruders pins
extern uint8_t active_extruder;


#endif

//Long pause
extern int saved_feedmultiply;
extern float HotendTempBckp;
extern int fanSpeedBckp;
extern float pause_lastpos[4];
extern unsigned long pause_time;
extern unsigned long start_pause_print;
extern unsigned long t_fan_rising_edge;

extern bool mesh_bed_leveling_flag;
extern bool mesh_bed_run_from_menu;

extern bool sortAlpha;

extern char dir_names[3][9];

// save/restore printing
extern bool saved_printing;

//save/restore printing in case that mmu is not responding
extern bool mmu_print_saved;

//estimated time to end of the print
extern uint8_t print_percent_done_normal;
extern uint32_t print_time_remaining_normal;
extern uint8_t print_percent_done_silent;
extern uint32_t print_time_remaining_silent;
#define PRINT_TIME_REMAINING_INIT 0xffffffff
#define PRINT_PERCENT_DONE_INIT   0xff
#define PRINTER_ACTIVE (IS_SD_PRINTING || is_usb_printing || isPrintPaused || (custom_message_type == 4) || saved_printing || (lcd_commands_type == LCD_COMMAND_V2_CAL) || card.paused || mmu_print_saved)

extern void calculate_extruder_multipliers();

// Similar to the default Arduino delay function, 
// but it keeps the background tasks running.
extern void delay_keep_alive(unsigned int ms);

extern void check_babystep();

extern void long_pause();

#ifdef DIS

void d_setup();
float d_ReadData();
void bed_analysis(float x_dimension, float y_dimension, int x_points_num, int y_points_num, float shift_x, float shift_y);

#endif
float temp_comp_interpolation(float temperature);
void temp_compensation_apply();
void temp_compensation_start();
void show_fw_version_warnings();
void erase_eeprom_section(uint16_t offset, uint16_t bytes);
uint8_t check_printer_version();

#ifdef PINDA_THERMISTOR
float temp_compensation_pinda_thermistor_offset(float temperature_pinda);
#endif //PINDA_THERMISTOR

void wait_for_heater(long codenum);
void serialecho_temperatures();
bool check_commands();

void uvlo_();
void uvlo_tiny();
void recover_print(uint8_t automatic); 
void setup_uvlo_interrupt();

#if defined(TACH_1) && TACH_1 >-1
void setup_fan_interrupt();
#endif

//extern void recover_machine_state_after_power_panic();
extern void recover_machine_state_after_power_panic(bool bTiny);
extern void restore_print_from_eeprom();
extern void position_menu();

extern void print_world_coordinates();
extern void print_physical_coordinates();
extern void print_mesh_bed_leveling_table();


//estimated time to end of the print
extern uint16_t print_time_remaining();
extern uint8_t print_percent_done();

#ifdef HOST_KEEPALIVE_FEATURE

// States for managing Marlin and host communication
// Marlin sends messages if blocked or busy
/*enum MarlinBusyState {
	NOT_BUSY,           // Not in a handler
	IN_HANDLER,         // Processing a GCode
	IN_PROCESS,         // Known to be blocking command input (as in G29)
	PAUSED_FOR_USER,    // Blocking pending any input
	PAUSED_FOR_INPUT    // Blocking pending text input (concept)
};*/

#define NOT_BUSY          1
#define IN_HANDLER        2
#define IN_PROCESS        3
#define PAUSED_FOR_USER   4
#define PAUSED_FOR_INPUT  5

#define KEEPALIVE_STATE(n) do { busy_state = n;} while (0)
extern void host_keepalive();
//extern MarlinBusyState busy_state;
extern int busy_state;

#endif //HOST_KEEPALIVE_FEATURE

#ifdef TMC2130

#define FORCE_HIGH_POWER_START	force_high_power_mode(true)
#define FORCE_HIGH_POWER_END	force_high_power_mode(false)

void force_high_power_mode(bool start_high_power_section);

#endif //TMC2130

// G-codes
void gcode_G28(bool home_x_axis, long home_x_value, bool home_y_axis, long home_y_value, bool home_z_axis, long home_z_value, bool calib, bool without_mbl);
void gcode_G28(bool home_x_axis, bool home_y_axis, bool home_z_axis);

bool gcode_M45(bool onlyZ, int8_t verbosity_level);
void gcode_M114();
void gcode_M701();

#ifdef UVLO_SUPPORT
	// Power panic - pin ZMAX on RAMPS
	#define UVLO !(PINE & (1<<4))
#endif //UVLO_SUPPORT

void proc_commands();


void M600_load_filament();
void M600_load_filament_movements();
void M600_wait_for_user();
void M600_check_state();