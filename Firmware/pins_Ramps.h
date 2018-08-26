/*****************************************************************
* Arduino RAMPS 1.4 Pin Assignments
******************************************************************/

#define ELECTRONICS "RAMPS_14EFB"

#define KNOWN_BOARD
#ifndef __AVR_ATmega2560__
  #error Oops!  Make sure you have 'Arduino Mega 2560' selected from the 'Tools -> Boards' menu.
#endif

//** MK3 features from here downwards **
//**************************************
//#define TMC2130		//uncomment if having this drivers installed

//#define AMBIENT_THERMISTOR

//#define W25X20CL                 // external 256kB flash
//#define BOOTAPP                  // bootloader support

/* POWER PANIC IS YET TO BE IMPLEMENTED */
//#define UVLO_SUPPORT	//already defined in "Configuration_prusa.h"

#define TACH_1				-1 

//** MK25 features from here downwards **
//***************************************
//#define PINDA_THERMISTOR

//#define SWI2C_SDA      20 //SDA on P3
//#define SWI2C_SCL      21 //SCL on P3
/* not used, added to satisfy code in temperature.cpp
* TODO: tape a thermistor to z probe and see if it works
* TODO: add another thermistor to ramps case and measure ambient temps */

//#define TEMP_AMBIENT_PIN	-1 
//#define TEMP_PINDA_PIN      -1 

//if errors, we should comment these 2 lines
//#define VOLT_PWR_PIN        -1 
//#define VOLT_BED_PIN        -1 

#define MOTOR_CURRENT_PWM_XY_PIN -1
#define MOTOR_CURRENT_PWM_Z_PIN  -1
#define MOTOR_CURRENT_PWM_E_PIN  -1

//#define TACH_0              -1 // !!! changed from 81 (EINY03)

// RAMPS pinout
#ifdef TMC2130
#define X_TMC2130_CS		59 //53?
#define X_TMC2130_DIAG		2
#endif

#define X_STEP_PIN			54
#define X_DIR_PIN			55
#define X_MIN_PIN			3
#define X_MAX_PIN			2  // Will be eventually assigned to UVLO for RAMPS
#define X_ENABLE_PIN		38
#define X_MS1_PIN			-1
#define X_MS2_PIN			-1

#ifdef TMC2130
#define Y_TMC2130_CS		64 //49?
#define Y_TMC2130_DIAG		15
#endif
#define Y_STEP_PIN			60
#define Y_DIR_PIN			61
#define Y_MIN_PIN			14
#define Y_MAX_PIN			15
#define Y_ENABLE_PIN		56
#define Y_MS1_PIN			-1
#define Y_MS2_PIN			-1

#ifdef TMC2130
#define Z_TMC2130_CS		44 //40?
#define Z_TMC2130_DIAG		19
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
#define TEMP_BED_PIN		 1 //14 //A14

#define HEATER_0_PIN		10
/*Array index for adc_samples[], not pin number */
#define TEMP_0_PIN			 0 //13 //A13

#define HEATER_1_PIN        -1
#ifdef PINDA_THERMISTOR
	#define TEMP_1_PIN       2 //15 //A15 (2 for adc conversion)
#else
	#define TEMP_1_PIN       15 //A15
#endif //PINDA_THERMISTOR

#define HEATER_2_PIN        -1
#define TEMP_2_PIN          -1

#ifdef TMC2130
#define E0_TMC2130_CS		63
#define E0_TMC2130_DIAG		40
#endif

#define E0_STEP_PIN         26
#define E0_DIR_PIN          28
#define E0_ENABLE_PIN       24
#define E0_MS1_PIN          -1
#define E0_MS2_PIN          -1

//values for G3D LCD panel!
#define SDPOWER             -1
#define SDSS                53
#define LED_PIN             13
#define FAN_PIN              9
#define FAN_1_PIN           -1
#define PS_ON_PIN           12
#define KILL_PIN            41  // 80 with Smart Controller LCD
#define SUICIDE_PIN         -1  // PIN that has to be turned on right after start, to keep power flowing.

//#define FR_SENS 4 // 21        //for mechanical filament sensor RAMPS

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

#if MOTHERBOARD != BOARD_RAMPS_14_EFB
	// Support for an 8 bit logic analyzer, for example the Saleae.
	// Channels 0-2 are fast, they could generate 2.667Mhz waveform with a software loop.
	#define LOGIC_ANALYZER_CH0		X_MIN_PIN		// PB6
	#define LOGIC_ANALYZER_CH1		Y_MIN_PIN		// PB5
	#define LOGIC_ANALYZER_CH2		53				// PB0 (PROC_nCS) //AUX3/SPI (LCD panel!)
	// Channels 3-7 are slow, they could generate
	// 0.889Mhz waveform with a software loop and interrupt locking,
	// 1.333MHz waveform without interrupt locking.
	#define LOGIC_ANALYZER_CH3 		73				// PJ3
	// PK0 has no Arduino digital pin assigned, so we set it directly.
	#define WRITE_LOGIC_ANALYZER_CH4(value) if (value) PORTK |= (1 << 0); else PORTK &= ~(1 << 0) // PK0
	#define LOGIC_ANALYZER_CH5		16				// PH0 (RXD2) //
	#define LOGIC_ANALYZER_CH6		17				// PH1 (TXD2) //
	#define LOGIC_ANALYZER_CH7 		76				// PJ5

	#define LOGIC_ANALYZER_CH0_ENABLE do { SET_OUTPUT(LOGIC_ANALYZER_CH0); WRITE(LOGIC_ANALYZER_CH0, false); } while (0)
	#define LOGIC_ANALYZER_CH1_ENABLE do { SET_OUTPUT(LOGIC_ANALYZER_CH1); WRITE(LOGIC_ANALYZER_CH1, false); } while (0)
	//#define LOGIC_ANALYZER_CH2_ENABLE do { SET_OUTPUT(LOGIC_ANALYZER_CH2); WRITE(LOGIC_ANALYZER_CH2, false); } while (0)
	#define LOGIC_ANALYZER_CH3_ENABLE do { SET_OUTPUT(LOGIC_ANALYZER_CH3); WRITE(LOGIC_ANALYZER_CH3, false); } while (0)
	#define LOGIC_ANALYZER_CH4_ENABLE do { DDRK |= 1 << 0; WRITE_LOGIC_ANALYZER_CH4(false); } while (0)
	#define LOGIC_ANALYZER_CH5_ENABLE do { cbi(UCSR2B, TXEN2); cbi(UCSR2B, RXEN2); cbi(UCSR2B, RXCIE2); SET_OUTPUT(LOGIC_ANALYZER_CH5); WRITE(LOGIC_ANALYZER_CH5, false); } while (0)
	#define LOGIC_ANALYZER_CH6_ENABLE do { cbi(UCSR2B, TXEN2); cbi(UCSR2B, RXEN2); cbi(UCSR2B, RXCIE2); SET_OUTPUT(LOGIC_ANALYZER_CH6); WRITE(LOGIC_ANALYZER_CH6, false); } while (0)
	#define LOGIC_ANALYZER_CH7_ENABLE do { SET_OUTPUT(LOGIC_ANALYZER_CH7); WRITE(LOGIC_ANALYZER_CH7, false); } while (0)

	//below code is only for MK3 and Einsy boards (no MK2.5 or below)
	#if MOTHERBOARD == BOARD_EINSY_1_0a //|| defined(TESTING)
		// Async output on channel 5 of the logical analyzer.
		// Baud rate 2MBit, 9 bits, 1 stop bit.
		#define LOGIC_ANALYZER_SERIAL_TX_ENABLE do { UBRR2H = 0; UBRR2L = 0; UCSR2B = (1 << TXEN2) | (1 << UCSZ02); UCSR2C = 0x06; } while (0)
		// Non-checked (quicker) variant. Use it if you are sure that the transmit buffer is already empty.
		#define LOGIC_ANALYZER_SERIAL_TX_WRITE_NC(C) do { if (C & 0x100) UCSR2B |= 1; else UCSR2B &= ~1; UDR2 = C; } while (0)
		#define LOGIC_ANALYZER_SERIAL_TX_WRITE(C) do { \
			/* Wait for empty transmit buffer */ \
			while (!(UCSR2A & (1<<UDRE2))); \
			/* Put data into buffer, sends the data */ \
			LOGIC_ANALYZER_SERIAL_TX_WRITE_NC(C); \
		} while (0)
	#endif
#endif //MOTHERBOARD == BOARD_RAMPS_14_EFB


