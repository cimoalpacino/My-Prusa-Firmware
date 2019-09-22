/*****************************************************************
* Arduino RAMPS 1.3/1.4 Pin Assignments
******************************************************************/

#define ELECTRONICS "RAMPS_14EFB"

#define KNOWN_BOARD
#ifndef __AVR_ATmega2560__
  #error Oops!  Make sure you have 'Arduino Mega 2560' selected from the 'Tools -> Boards' menu.
#endif

//#define PINDA_THERMISTOR
//#define SWI2C_SDA      20 //SDA (already defined in config.h)
//#define SWI2C_SCL      21 //SCL (already defined in config.h)

// RAMPS pinout
#ifdef TMC2130
#define X_TMC2130_CS		53
#define X_TMC2130_DIAG		X_MAX_PIN //-1 // NO DIAG OUTPUTS
#endif
#define X_STEP_PIN			54
#define X_DIR_PIN			55
#define X_MIN_PIN			3
#define X_MAX_PIN			2  // Will be eventually assigned to UVLO for RAMPS
#define X_ENABLE_PIN		38
#define X_MS1_PIN			-1
#define X_MS2_PIN			-1

#ifdef TMC2130
#define Y_TMC2130_CS		49
#define Y_TMC2130_DIAG		Y_MAX_PIN //-1 // NO DIAG OUTPUTS
#endif
#define Y_STEP_PIN			60
#define Y_DIR_PIN			61
#define Y_MIN_PIN			14
#define Y_MAX_PIN			15
#define Y_ENABLE_PIN		56
#define Y_MS1_PIN			-1
#define Y_MS2_PIN			-1

#ifdef TMC2130
#define Z_TMC2130_CS		40
#define Z_TMC2130_DIAG		Z_MAX_PIN //-1 // NO DIAG OUTPUTS
#endif
#define Z_STEP_PIN			46
#define Z_DIR_PIN			48
#define Z_MIN_PIN			18
#define Z_MAX_PIN			19
#define Z_ENABLE_PIN		62
#define Z_MS1_PIN			-1
#define Z_MS2_PIN			-1

#define HEATER_BED_PIN		 8
/*Array index for adc_samples[], not pin number */
#define TEMP_BED_PIN		 14 //A14 (not equal to digital Y_MIN_PIN!)

#define HEATER_0_PIN		10
/*Array index for adc_samples[], not pin number */
#define TEMP_0_PIN			 13 //A13 (not equal to digital LED_PIN!)

#define HEATER_1_PIN        -1
#ifdef PINDA_THERMISTOR
	#define TEMP_1_PIN       15 //A15 (not equal to digital Y_MAX_PIN!)
#else
	#define TEMP_1_PIN      -1
#endif //PINDA_THERMISTOR

#define HEATER_2_PIN        -1
#define TEMP_2_PIN          -1

/*test*/
//#define TEMP_AMBIENT_PIN	-1
//#define TEMP_PINDA_PIN    -1

#ifdef TMC2130
#define E0_TMC2130_CS		42
#define E0_TMC2130_DIAG		-1 // NO DIAG OUTPUTS
#endif
#define E0_STEP_PIN         26
#define E0_DIR_PIN          28
#define E0_ENABLE_PIN       24
#define E0_MS1_PIN          -1
#define E0_MS2_PIN          -1

//#define MOTOR_CURRENT_PWM_XY_PIN -1
//#define MOTOR_CURRENT_PWM_Z_PIN  -1
//#define MOTOR_CURRENT_PWM_E_PIN  -1

#ifdef G3D_PANEL
	//values for G3D LCD panel
	#define SDPOWER             -1
	#define SDSS                53
	#define LED_PIN             13
	#define FAN_PIN              9
	#define FAN_1_PIN           -1
	#define PS_ON_PIN           12
	#define KILL_PIN            41  
	#define SUICIDE_PIN         -1  // PIN that has to be turned on right after start, to keep power flowing.
	#define TACH_0				-1  //30	// noctua extruder fan

	#define BEEPER			33 //-1 to disable
	#define LCD_PINS_RS		16
	#define LCD_PINS_ENABLE	17
	#define LCD_PINS_D4		23
	#define LCD_PINS_D5		25
	#define LCD_PINS_D6		27
	#define LCD_PINS_D7		29

	//buttons are directly attached using AUX-2
	#define BTN_EN1                37
	#define BTN_EN2                35
	#define BTN_ENC                31 // the click
	#define SDCARDDETECT           49
#else
	//values for REPRAP_DISCOUNT_SMART_CONTROLLER
	#define SDPOWER             -1
	#define SDSS                53
	#define LED_PIN             13
	#define FAN_PIN              9
	#define FAN_1_PIN           -1
	#define PS_ON_PIN           -1
	#define KILL_PIN            41  //
	#define SUICIDE_PIN         -1  // PIN that has to be turned on right after start, to keep power flowing.
	#define TACH_0				-1 //30	// noctua extruder fan

	#define BEEPER              37  // Beeper
	#define LCD_PINS_RS         40
	#define LCD_PINS_ENABLE     42
	#define LCD_PINS_D4	        65
	#define LCD_PINS_D5         66
	#define LCD_PINS_D6         44
	#define LCD_PINS_D7         64

	//buttons are directly attached using AUX-2
	#define BTN_EN1                31
	#define BTN_EN2                33
	#define BTN_ENC                35  // the click
	#define SDCARDDETECT           49
#endif

/*RAMPS*/
//#ifndef IR_SENSOR
	#define IR_SENSOR_PIN 5 //IR sensor on RAMPS pin D5 of servo motors pins
//#endif // FR_SENS
