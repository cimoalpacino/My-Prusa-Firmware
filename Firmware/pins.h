#ifndef PINS_H
#define PINS_H

#include "boards.h"
#include "Configuration_prusa.h"
#include "Configuration.h"

#define LARGE_FLASH true

/****************************************************************************************
* Arduino Mega pin assignment
*
****************************************************************************************/
#if IS_RAMPS || MB(3DRAG) || MB(AZTEEG_X3) || MB(AZTEEG_X3_PRO)
#define KNOWN_BOARD 1

//////////////////FIX THIS//////////////
#ifndef __AVR_ATmega1280__
 #ifndef __AVR_ATmega2560__
 #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
 #endif
#endif


// uncomment one of the following lines for RAMPS v1.3 or v1.0, comment both for v1.2 or 1.1
// #define RAMPS_V_1_3
// #define RAMPS_V_1_0


#if (IS_RAMPS && !MB(RAMPS_OLD)) || MB(3DRAG) || MB(AZTEEG_X3) || MB(AZTEEG_X3_PRO)

  #define LARGE_FLASH true

  #define ELECTRONICS "RAMPS1.4"

  #if MB(3DRAG)
    #define X_STEP_PIN         54
    #define X_DIR_PIN          55
    #define X_ENABLE_PIN       38
    #define X_MIN_PIN           3
    #define X_MAX_PIN           -1   //2 //Max endstops default to disabled "-1", set to commented value to enable.

    #define Y_STEP_PIN         60
    #define Y_DIR_PIN          61
    #define Y_ENABLE_PIN       56
    #define Y_MIN_PIN          14
    #define Y_MAX_PIN          -1   //15

    #define Z_STEP_PIN         46
    #define Z_DIR_PIN          48
    #define Z_ENABLE_PIN       63
    #define Z_MIN_PIN          18
    #define Z_MAX_PIN          -1

    #define Y2_STEP_PIN        36
    #define Y2_DIR_PIN         34
    #define Y2_ENABLE_PIN      30

    #define Z2_STEP_PIN        36
    #define Z2_DIR_PIN         34
    #define Z2_ENABLE_PIN      30

    #define E0_STEP_PIN        26
    #define E0_DIR_PIN         28
    #define E0_ENABLE_PIN      24

    #define E1_STEP_PIN        36
    #define E1_DIR_PIN         34
    #define E1_ENABLE_PIN      30

    #define SDPOWER            -1
    #define SDSS               25//53
    #define LED_PIN            13

    #define BEEPER             33

  #else

    #define X_STEP_PIN         54
    #define X_DIR_PIN          55
    #define X_ENABLE_PIN       38
    #define X_MIN_PIN           3
    #define X_MAX_PIN           2
    #define X_MS1_PIN          -1
    #define X_MS2_PIN          -1

    #define Y_STEP_PIN         60
    #define Y_DIR_PIN          61
    #define Y_ENABLE_PIN       56
    #define Y_MIN_PIN          14
    #define Y_MAX_PIN          15
    #define Y_MS1_PIN          -1
    #define Y_MS2_PIN          -1

    #define Z_STEP_PIN         46
    #define Z_DIR_PIN          48
    #define Z_ENABLE_PIN       62
    #define Z_MIN_PIN          18
    #define Z_MAX_PIN          19
    #define Z_MS1_PIN          -1
    #define Z_MS2_PIN          -1

    #define Y2_STEP_PIN        36
    #define Y2_DIR_PIN         34
    #define Y2_ENABLE_PIN      30

    #define Z2_STEP_PIN        36
    #define Z2_DIR_PIN         34
    #define Z2_ENABLE_PIN      30

    #define E0_STEP_PIN        26
    #define E0_DIR_PIN         28
    #define E0_ENABLE_PIN      24

    #define E0_MS1_PIN          -1
    #define E0_MS2_PIN          -1

    #define E1_STEP_PIN        36
    #define E1_DIR_PIN         34
    #define E1_ENABLE_PIN      30

    #define FR_SENS 4 // 21        //dodal 27.5.2018

    // Stepper chip selects
    #define X_CS_PIN           63
    #define Y_CS_PIN           40
    #define Z_CS_PIN           42
    #define E0_CS_PIN          65
    #define E1_CS_PIN          44

#if MB(RAMPS_13_EEB)  //FMM added for Filament Extruder
#ifdef FILAMENT_SENSOR
    //define analog pin for the filament width sensor input
    //Use the RAMPS 1.4 Analog input 5 on the AUX2 connector
      #define FILWIDTH_PIN        5
#endif
#endif


    #if MB(AZTEEG_X3_PRO)
      #define E2_STEP_PIN        23
      #define E2_DIR_PIN         25
      #define E2_ENABLE_PIN      40

      #define E3_STEP_PIN        27
      #define E3_DIR_PIN         29
      #define E3_ENABLE_PIN      41

      #define E4_STEP_PIN        43
      #define E4_DIR_PIN         37
      #define E4_ENABLE_PIN      42
    #endif

    #define SDPOWER            -1
    #define SDSS               53
    #define LED_PIN            13
  #endif

  #if MB(RAMPS_13_EFB) || MB(RAMPS_13_EFF) || MB(AZTEEG_X3)
    #define FAN_PIN            9 // (Sprinter config)
  #elif MB(AZTEEG_X3_PRO)
    #define FAN_PIN            11 // Last Heater Pin on board
  #else
    #define FAN_PIN            4 // IO pin. Buffer needed
  #endif

  #if MB(3DRAG) || MB(RAMPS_13_EEF)
    #define FAN_PIN            8
  #endif

  #if MB(RAMPS_13_EFF)
    #define CONTROLLERFAN_PIN  -1 //Pin used for the fan to cool controller
  #endif

  #define PS_ON_PIN          12

  #if defined(REPRAP_DISCOUNT_SMART_CONTROLLER) || defined(G3D_PANEL)
    #define KILL_PIN           41
  #else
    #define KILL_PIN           -1
  #endif

  #if MB(RAMPS_13_EFF)
    #define HEATER_0_PIN       8
  #else
    #define HEATER_0_PIN       10   // EXTRUDER 1
  #endif

  #if MB(RAMPS_13_EFB) || MB(AZTEEG_X3)
    #define HEATER_1_PIN       -1
  #else
    #define HEATER_1_PIN       9    // EXTRUDER 2 (FAN On Sprinter)
  #endif


  #if MB(3DRAG)
    #define HEATER_0_PIN       10
    #define HEATER_1_PIN       12
    #define HEATER_2_PIN       6
  #elif MB(AZTEEG_X3_PRO)
    #define HEATER_2_PIN       16
    #define HEATER_3_PIN       17
    #define HEATER_4_PIN       4
    #define HEATER_5_PIN       5
    #define HEATER_6_PIN       6
    #define HEATER_7_PIN       11
  #else
    #define HEATER_2_PIN       -1
  #endif

  #define TEMP_0_PIN         13   // ANALOG NUMBERING
  #define TEMP_1_PIN         15   // ANALOG NUMBERING
  #if MB(AZTEEG_X3_PRO)
    #define TEMP_2_PIN         12   // ANALOG NUMBERING
    #define TEMP_3_PIN         11   // ANALOG NUMBERING
    #define TEMP_4_PIN         10   // ANALOG NUMBERING
    #define TC1                4    // ANALOG NUMBERING Thermo couple on Azteeg X3Pro
    #define TC2                5    // ANALOG NUMBERING Thermo couple on Azteeg X3Pro
  #else
    #define TEMP_2_PIN         -1   // ANALOG NUMBERING
  #endif

  #if MB(RAMPS_13_EFF) || MB(RAMPS_13_EEF)
    #define HEATER_BED_PIN     -1    // NO BED
  #else
    #if MB(3DRAG)
      #define HEATER_BED_PIN     9    // BED
    #else
      #define HEATER_BED_PIN     8    // BED
    #endif
  #endif

  #define TEMP_BED_PIN       14   // ANALOG NUMBERING

  #ifdef NUM_SERVOS
    #define SERVO0_PIN         11

    #if NUM_SERVOS > 1
      #define SERVO1_PIN         6
    #endif

    #if NUM_SERVOS > 2
      #define SERVO2_PIN         5
    #endif

    #if NUM_SERVOS > 3
      #define SERVO3_PIN         4
    #endif
  #endif

  #if MB(AZTEEG_X3_PRO)
    #define BEEPER 33
  #endif

  #ifdef TEMP_STAT_LEDS
    #if MB(AZTEEG_X3)
      #define STAT_LED_RED       6
      #define STAT_LED_BLUE     11
    #endif
  #endif

  #ifdef ULTRA_LCD

    #ifdef NEWPANEL
      #define LCD_PINS_RS 16
      #define LCD_PINS_ENABLE 17
      #define LCD_PINS_D4 23
      #define LCD_PINS_D5 25
      #define LCD_PINS_D6 27
      #define LCD_PINS_D7 29

      #ifdef REPRAP_DISCOUNT_SMART_CONTROLLER
        #define BEEPER 37

        #define BTN_EN1 31
        #define BTN_EN2 33
        #define BTN_ENC 35

        #define SDCARDDETECT 49
      #elif defined(LCD_I2C_PANELOLU2)
        #define BTN_EN1 47  //reverse if the encoder turns the wrong way.
        #define BTN_EN2 43
        #define BTN_ENC 32
        #define LCD_SDSS 53
        #define SDCARDDETECT -1
        #define KILL_PIN 41
      #elif defined(LCD_I2C_VIKI)
        #define BTN_EN1 22  //reverse if the encoder turns the wrong way.
        #define BTN_EN2 7
        #define BTN_ENC -1
        #define LCD_SDSS 53
        #define SDCARDDETECT 49
      #else
        //arduino pin which triggers an piezzo beeper
        #define BEEPER 33  // Beeper on AUX-4

        //buttons are directly attached using AUX-2
        #ifdef REPRAPWORLD_KEYPAD
          #define BTN_EN1 64 // encoder
          #define BTN_EN2 59 // encoder
          #define BTN_ENC 63 // enter button
          #define SHIFT_OUT 40 // shift register
          #define SHIFT_CLK 44 // shift register
          #define SHIFT_LD 42 // shift register
        #else
          #define BTN_EN1 37
          #define BTN_EN2 35
          #define BTN_ENC 31  //the click
        #endif

        #ifdef G3D_PANEL
          #define SDCARDDETECT 49
        #else
          #define SDCARDDETECT -1  // Ramps does not use this port
        #endif

      #endif

      #if MB(3DRAG)
        #define BEEPER -1

        #define LCD_PINS_RS 27
        #define LCD_PINS_ENABLE 29
        #define LCD_PINS_D4 37
        #define LCD_PINS_D5 35
        #define LCD_PINS_D6 33
        #define LCD_PINS_D7 31

       //buttons
       #define BTN_EN1 16
       #define BTN_EN2 17
       #define BTN_ENC 23 //the click

    #endif
    #else //old style panel with shift register
      //arduino pin witch triggers an piezzo beeper
      #define BEEPER 33   //No Beeper added

      //buttons are attached to a shift register
  // Not wired this yet
      //#define SHIFT_CLK 38
      //#define SHIFT_LD 42
      //#define SHIFT_OUT 40
      //#define SHIFT_EN 17

      #define LCD_PINS_RS 16
      #define LCD_PINS_ENABLE 17
      #define LCD_PINS_D4 23
      #define LCD_PINS_D5 25
      #define LCD_PINS_D6 27
      #define LCD_PINS_D7 29
    #endif
  #endif //ULTRA_LCD

#else // RAMPS_V_1_1 or RAMPS_V_1_2 as default (BOARD_RAMPS_OLD)

#define X_STEP_PIN         26
#define X_DIR_PIN          28
#define X_ENABLE_PIN       24
#define X_MIN_PIN           3
#define X_MAX_PIN          -1    //2

#define Y_STEP_PIN         38
#define Y_DIR_PIN          40
#define Y_ENABLE_PIN       36
#define Y_MIN_PIN          16
#define Y_MAX_PIN          -1    //17

#define Z_STEP_PIN         44
#define Z_DIR_PIN          46
#define Z_ENABLE_PIN       42
#define Z_MIN_PIN          18
#define Z_MAX_PIN          -1    //19

#define E0_STEP_PIN         32
#define E0_DIR_PIN          34
#define E0_ENABLE_PIN       30

#define SDPOWER            48
#define SDSS               53
#define LED_PIN            13
#define PS_ON_PIN          -1
#define KILL_PIN           -1

#ifdef RAMPS_V_1_0 // RAMPS_V_1_0
  #define HEATER_0_PIN     12    // RAMPS 1.0
  #define HEATER_BED_PIN   -1    // RAMPS 1.0
  #define FAN_PIN          11    // RAMPS 1.0
#else // RAMPS_V_1_1 or RAMPS_V_1_2
  #define HEATER_0_PIN     10    // RAMPS 1.1
  #define HEATER_BED_PIN    8    // RAMPS 1.1
  #define FAN_PIN           9    // RAMPS 1.1
#endif
#define HEATER_1_PIN        -1
#define HEATER_2_PIN        -1
#define TEMP_0_PIN          2    // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!
#define TEMP_1_PIN          -1
#define TEMP_2_PIN          -1
#define TEMP_BED_PIN        1    // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!

#endif // RAMPS_13_EFB || RAMPS_13_EEB || RAMPS_13_EFF || 3DRAG

// SPI for Max6675 Thermocouple

#ifndef SDSUPPORT
// these pins are defined in the SD library if building with SD support
  #define SCK_PIN          52
  #define MISO_PIN         50
  #define MOSI_PIN         51
  #define MAX6675_SS       66// Do not use pin 53 if there is even the remote possibility of using Dsplay/SD card
#else
  #define MAX6675_SS       66// Do not use pin 49 as this is tied to the switch inside the SD card socket to detect if there is an SD card present
#endif

#endif // RAMPS_OLD || RAMPS_13_EFB || RAMPS_13_EEB || RAMPS_13_EFF || 3DRAG

/*****************************************************************
* Rambo Pin Assignments 1.3
******************************************************************/
#if MOTHERBOARD == 302
  #define MINI_RAMBO

#endif
#if MOTHERBOARD == 301 || MOTHERBOARD == 302
  #define KNOWN_BOARD
  #ifndef __AVR_ATmega2560__
    #error Oops!  Make sure you have 'Arduino Mega 2560' selected from the 'Tools -> Boards' menu.
  #endif


  #define FR_SENS 4 // 21


  #define X_STEP_PIN 37
  #define X_DIR_PIN 48
  #define X_ENABLE_PIN 29
  #define X_MS1_PIN 40
  #define X_MS2_PIN 41
  #define Y_STEP_PIN 36
  #define Y_DIR_PIN 49
  #define Y_ENABLE_PIN 28
  #define Y_MS1_PIN 69
  #define Y_MS2_PIN 39
  #define Z_STEP_PIN 35
  #define Z_DIR_PIN 47
  #define Z_ENABLE_PIN 27
  #define Z_MS1_PIN 68
  #define Z_MS2_PIN 67
  #define TEMP_BED_PIN 2
  #define TEMP_0_PIN 0
  #define HEATER_1_PIN 7
  #define TEMP_1_PIN 1
  #define TEMP_2_PIN -1

#ifndef DISABLE_MAX_ENDSTOPS
  #define X_MAX_PIN 30
  #define Z_MAX_PIN 23
  #define Y_MAX_PIN 24
#else
  #define X_MAX_PIN -1
  #define Y_MAX_PIN -1
  #define Z_MAX_PIN -1
#endif

#ifndef DISABLE_MIN_ENDSTOPS
  #define X_MIN_PIN 12
  #define Y_MIN_PIN 11
  #define Z_MIN_PIN 10
#else
  #define X_MIN_PIN -1
  #define Y_MIN_PIN -1
  #define Z_MIN_PIN -1
#endif


#ifdef SNMM

#define E_MUX0_PIN 17
#define E_MUX1_PIN 16


#endif

#ifdef DIS
#define D_REQUIRE 30
#define D_DATA 20
#define D_DATACLOCK 21

#endif

// The SDSS pin uses a different pin mapping from file Sd2PinMap.h
#define SDSS               53

#ifndef SDSUPPORT
// these pins are defined in the SD library if building with SD support
  #define SCK_PIN           52
  #define MISO_PIN         50
  #define MOSI_PIN         51
#endif

    #define BEEPER 84

        #define BTN_EN1 72
        #define BTN_EN2 14
        #define BTN_ENC 9

        #define SDCARDDETECT 15

        #define LCD_PINS_RS 82
        #define LCD_PINS_ENABLE 18
        #define LCD_PINS_D4 19
        #define LCD_PINS_D5 70
        #define LCD_PINS_D6 85
        #define LCD_PINS_D7 71



  #define E0_STEP_PIN         34
  #define E0_DIR_PIN          43
  #define E0_ENABLE_PIN       26
  #define E0_MS1_PIN 65
  #define E0_MS2_PIN 66
  #define LED_PIN            13
  #ifdef THREEMM_PRINTER
      #define FAN_PIN            8
  #else
      #define FAN_PIN            6
  #endif
  #define KILL_PIN           -1 //80 with Smart Controller LCD
  #define SUICIDE_PIN        -1  //PIN that has to be turned on right after start, to keep power flowing.
  #define SDPOWER            -1
  #define HEATER_2_PIN -1
  #ifdef MINI_RAMBO

    #define ELECTRONICS "RAMBo13a"

    #define HEATER_0_PIN 3
    #define HEATER_BED_PIN 4
    #define FAN_1_PIN -1 //6
    #define PS_ON_PIN 71
    #define MOTOR_CURRENT_PWM_XY_PIN 46
    #define MOTOR_CURRENT_PWM_Z_PIN 45
    #define MOTOR_CURRENT_PWM_E_PIN 44

  #else //RAMBo
    #define ELECTRONICS "RAMBoBig"

    #define E1_STEP_PIN         33
    #define E1_DIR_PIN          42
    #define E1_ENABLE_PIN       25
    #define E1_MS1_PIN 63
    #define E1_MS2_PIN 64
    #define DIGIPOTSS_PIN 38
    #define DIGIPOT_CHANNELS {4,5,3,0,1} // X Y Z E0 E1 digipot channels to stepper driver mapping
    #define HEATER_0_PIN  9
    #define HEATER_BED_PIN 3
    #define PS_ON_PIN          4
    #define SDSS               53
    #ifdef ULTRA_LCD
      #define KILL_PIN 80
      #ifdef NEWPANEL
        //arduino pin which triggers an piezzo beeper
        #define BEEPER 84      // Beeper on AUX-4
        #define LCD_PINS_RS 82
        #define LCD_PINS_ENABLE 18
        #define LCD_PINS_D4 19
        #define LCD_PINS_D5 70
        #define LCD_PINS_D6 85
        #define LCD_PINS_D7 71
        //buttons are directly attached using AUX-2
        #define BTN_EN1 76
        #define BTN_EN2 77
        #define BTN_ENC 78  //the click
        #define BLEN_C 2
        #define BLEN_B 1
        #define BLEN_A 0
        #define SDCARDDETECT 81    // Ramps does not use this port
        //encoder rotation values
        #define encrot0 0
        #define encrot1 2
        #define encrot2 3
        #define encrot3 1
      #else //old style panel with shift register
        //arduino pin witch triggers an piezzo beeper
        #define BEEPER 84    //No Beeper added
        //buttons are attached to a shift register
        // Not wired this yet
        // #define SHIFT_CLK 38
        // #define SHIFT_LD 42
        // #define SHIFT_OUT 40
        // #define SHIFT_EN 17
        #define LCD_PINS_RS 82
        #define LCD_PINS_ENABLE 18
        #define LCD_PINS_D4 19
        #define LCD_PINS_D5 70
        #define LCD_PINS_D6 85
        #define LCD_PINS_D7 71
        //encoder rotation values
        #define encrot0 0
        #define encrot1 2
        #define encrot2 3
        #define encrot3 1
        //bits in the shift register that carry the buttons for:
        // left up center down right red
        #define BL_LE 7
        #define BL_UP 6
        #define BL_MI 5
        #define BL_DW 4
        #define BL_RI 3
        #define BL_ST 2
        #define BLEN_B 1
        #define BLEN_A 0
      #endif
    #endif //ULTRA_LCD
  #endif //RAMBo/MiniRambo option
#endif








/*****************************************************************
* Rambo mini Pin Assignments 1.0
******************************************************************/
#if MOTHERBOARD == 102
  #define ELECTRONICS "RAMBo10a"
  #define KNOWN_BOARD
  #ifndef __AVR_ATmega2560__
    #error Oops!  Make sure you have 'Arduino Mega 2560' selected from the 'Tools -> Boards' menu.
  #endif

  #define FR_SENS 4 // 21

#ifdef SNMM

#define E_MUX0_PIN 17
#define E_MUX1_PIN 16


#endif
  #define LARGE_FLASH true
  #define X_STEP_PIN 37
  #define X_DIR_PIN 48
  #define X_ENABLE_PIN 29
  #define X_MS1_PIN 40
  #define X_MS2_PIN 41
  #define Y_STEP_PIN 36
  #define Y_DIR_PIN 49
  #define Y_ENABLE_PIN 28
  #define Y_MS1_PIN 69
  #define Y_MS2_PIN 39
  #define Z_STEP_PIN 35
  #define Z_DIR_PIN 47
  #define Z_ENABLE_PIN 27
  #define Z_MS1_PIN 68
  #define Z_MS2_PIN 67
  #define TEMP_BED_PIN 2
  #define TEMP_0_PIN 0
  #define HEATER_1_PIN 7
  #define TEMP_1_PIN 1
  #define TEMP_2_PIN -1

#ifndef DISABLE_MAX_ENDSTOPS
  #define X_MAX_PIN 30
  #define Z_MAX_PIN 23
  #define Y_MAX_PIN 24
#else
  #define X_MAX_PIN -1
  #define Y_MAX_PIN -1
  #define Z_MAX_PIN -1
#endif

#ifndef DISABLE_MIN_ENDSTOPS
  #define X_MIN_PIN 12
  #define Y_MIN_PIN 11
  #define Z_MIN_PIN 10
#else
  #define X_MIN_PIN -1
  #define Y_MIN_PIN -1
  #define Z_MIN_PIN -1
#endif

  // The SDSS pin uses a different pin mapping from file Sd2PinMap.h
#define SDSS               53

#ifndef SDSUPPORT
// these pins are defined in the SD library if building with SD support
  #define SCK_PIN           52
  #define MISO_PIN         50
  #define MOSI_PIN         51
#endif

    #define BEEPER 78

        #define BTN_EN1 80
        #define BTN_EN2 73
        #define BTN_ENC 21

        #define SDCARDDETECT 72

        #define LCD_PINS_RS 38
        #define LCD_PINS_ENABLE 5
        #define LCD_PINS_D4 14
        #define LCD_PINS_D5 15
        #define LCD_PINS_D6 32
        #define LCD_PINS_D7 31



  #define E0_STEP_PIN         34
  #define E0_DIR_PIN          43
  #define E0_ENABLE_PIN       26
  #define E0_MS1_PIN 65
  #define E0_MS2_PIN 66
  #define LED_PIN            13
  #ifdef THREEMM_PRINTER
      #define FAN_PIN            8
  #else
      #define FAN_PIN            6
  #endif
  #define KILL_PIN           -1 //80 with Smart Controller LCD
  #define SUICIDE_PIN        -1  //PIN that has to be turned on right after start, to keep power flowing.
  #define SDPOWER            -1
  #define HEATER_2_PIN -1

    #define HEATER_0_PIN 3
    #define HEATER_BED_PIN 4
    #define FAN_1_PIN -1 //6
    #define PS_ON_PIN 71
    #define MOTOR_CURRENT_PWM_XY_PIN 46
    #define MOTOR_CURRENT_PWM_Z_PIN 45
    #define MOTOR_CURRENT_PWM_E_PIN 44


#endif





#ifndef KNOWN_BOARD
#error Unknown MOTHERBOARD value in configuration.h
#endif

//List of pins which to ignore when asked to change by gcode, 0 and 1 are RX and TX, do not mess with those!
#define _E0_PINS E0_STEP_PIN, E0_DIR_PIN, E0_ENABLE_PIN, HEATER_0_PIN,
#if EXTRUDERS > 1
  #define _E1_PINS E1_STEP_PIN, E1_DIR_PIN, E1_ENABLE_PIN, HEATER_1_PIN,
#else
  #define _E1_PINS
#endif
#if EXTRUDERS > 2
  #define _E2_PINS E2_STEP_PIN, E2_DIR_PIN, E2_ENABLE_PIN, HEATER_2_PIN,
#else
  #define _E2_PINS
#endif

#ifdef X_STOP_PIN
  #if X_HOME_DIR < 0
    #define X_MIN_PIN X_STOP_PIN
    #define X_MAX_PIN -1
  #else
    #define X_MIN_PIN -1
    #define X_MAX_PIN X_STOP_PIN
  #endif
#endif

#ifdef Y_STOP_PIN
  #if Y_HOME_DIR < 0
    #define Y_MIN_PIN Y_STOP_PIN
    #define Y_MAX_PIN -1
  #else
    #define Y_MIN_PIN -1
    #define Y_MAX_PIN Y_STOP_PIN
  #endif
#endif

#ifdef Z_STOP_PIN
  #if Z_HOME_DIR < 0
    #define Z_MIN_PIN Z_STOP_PIN
    #define Z_MAX_PIN -1
  #else
    #define Z_MIN_PIN -1
    #define Z_MAX_PIN Z_STOP_PIN
  #endif
#endif


#define SENSITIVE_PINS {0, 1, X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_MIN_PIN, X_MAX_PIN, Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_MIN_PIN, Y_MAX_PIN, Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_MIN_PIN, Z_MAX_PIN, PS_ON_PIN, \
                        HEATER_BED_PIN, FAN_PIN,                  \
                        _E0_PINS _E1_PINS _E2_PINS             \
                        analogInputToDigitalPin(TEMP_0_PIN), analogInputToDigitalPin(TEMP_1_PIN), analogInputToDigitalPin(TEMP_2_PIN), analogInputToDigitalPin(TEMP_BED_PIN) }

#endif //__PINS_H

