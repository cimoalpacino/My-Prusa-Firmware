//sm4.c - simple 4-axis stepper control

#include "sm4.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <math.h>
#include "Arduino.h"

#include "boards.h"
#define false 0
#define true 1
#include "Configuration_prusa.h"


#ifdef NEW_XYZCAL


// Signal pinouts

// direction signal - MiniRambo
//#define X_DIR_PIN    48 //PL1 (-)
//#define Y_DIR_PIN    49 //PL0 (-)
//#define Z_DIR_PIN    47 //PL2 (-)
//#define E0_DIR_PIN   43 //PL6 (+)

//direction signal - EinsyRambo
//#define X_DIR_PIN    49 //PL0 (+)
//#define Y_DIR_PIN    48 //PL1 (-)
//#define Z_DIR_PIN    47 //PL2 (+)
//#define E0_DIR_PIN   43 //PL6 (-)

//step signal pinout - common for all rambo boards
//#define X_STEP_PIN   37 //PC0 (+)
//#define Y_STEP_PIN   36 //PC1 (+)
//#define Z_STEP_PIN   35 //PC2 (+)
//#define E0_STEP_PIN  34 //PC3 (+)

#if (MOTHERBOARD == BOARD_RAMPS_14_EFB)
	//direction signal - RAMPS
	//#define X_DIR_PIN    96 //A1  = PF1	0 (-) #define INVERT_X_DIR 0
	//#define Y_DIR_PIN    90 //A7  = PF7	0 (-) #define INVERT_Y_DIR 0
	//#define Z_DIR_PIN    36 //D48 = PL1	0 (-) #define INVERT_Z_DIR 0
	//#define E0_DIR_PIN   72 //D28 = PA6	1 (+) #define INVERT_E_DIR 1

	//step signal pinout - RAMPS (Configuration_adv.h)
	//#define X_STEP_PIN   97 //A0  = PF0	0 (-)
	//#define Y_STEP_PIN   91 //A6  = PF6	0 (-)
	//#define Z_STEP_PIN   38 //D46 = PL3	0 (-)
	//#define E0_STEP_PIN  74 //D26 = PA4	0 (-)
#endif
/*RAMPS*/
#define X_DIR INVERT_X_DIR:!INVERT_X_DIR
#define Y_DIR INVERT_Y_DIR:!INVERT_Y_DIR
#define Z_DIR INVERT_Z_DIR:!INVERT_Z_DIR
#define E_DIR INVERT_E0_DIR:!INVERT_E0_DIR
uint8_t dir_mask = 0x0F^(INVERT_X_DIR | (INVERT_Y_DIR << 1) | (INVERT_Z_DIR << 2) | (INVERT_E0_DIR << 3));
/*RAMPS*/

sm4_stop_cb_t sm4_stop_cb = 0;

sm4_update_pos_cb_t sm4_update_pos_cb = 0;

sm4_calc_delay_cb_t sm4_calc_delay_cb = 0;

uint16_t sm4_cpu_time = 0;


uint8_t sm4_get_dir(uint8_t axis)
{
	switch (axis)
	{
#if ((MOTHERBOARD == BOARD_RAMBO_MINI_1_0) || (MOTHERBOARD == BOARD_RAMBO_MINI_1_3))
	case 0: return (PORTL & 2)?0:1;
	case 1: return (PORTL & 1)?0:1;
	case 2: return (PORTL & 4)?0:1;
	case 3: return (PORTL & 64)?1:0;
#elif ((MOTHERBOARD == BOARD_EINSY_1_0a))
	case 0: return (PORTL & 1)?1:0;
	case 1: return (PORTL & 2)?0:1;
	case 2: return (PORTL & 4)?1:0;
	case 3: return (PORTL & 64)?0:1;
/*RAMPS*/
#elif (MOTHERBOARD == BOARD_RAMPS_14_EFB)
	case 0: return (PORTF & 2)  ?X_DIR; //0 : 1;	//1 -
	case 1: return (PORTF & 128)?Y_DIR; //0 : 1;	//7 -
	case 2: return (PORTL & 2)  ?Z_DIR; //0 : 1;	//1 -
	case 3: return (PORTA & 64) ?E_DIR; //1 : 0;	//6 +
#endif
	}
	return 0;
}

void sm4_set_dir(uint8_t axis, uint8_t dir)
{
	switch (axis)
	{
#if ((MOTHERBOARD == BOARD_RAMBO_MINI_1_0) || (MOTHERBOARD == BOARD_RAMBO_MINI_1_3))
	case 0: if (!dir) PORTL |= 2; else PORTL &= ~2; break;
	case 1: if (!dir) PORTL |= 1; else PORTL &= ~1; break;
	case 2: if (!dir) PORTL |= 4; else PORTL &= ~4; break;
	case 3: if (dir) PORTL |= 64; else PORTL &= ~64; break;
#elif ((MOTHERBOARD == BOARD_EINSY_1_0a))
	case 0: if (dir) PORTL |= 1; else PORTL &= ~1; break;
	case 1: if (!dir) PORTL |= 2; else PORTL &= ~2; break;
	case 2: if (dir) PORTL |= 4; else PORTL &= ~4; break;
	case 3: if (!dir) PORTL |= 64; else PORTL &= ~64; break;
/*RAMPS*/
#elif (MOTHERBOARD == BOARD_RAMPS_14_EFB)
	case 0: if (/*!dir)*/dir == INVERT_X_DIR) PORTF |= 2; else PORTF &= ~2; break;		//1 -
	case 1: if (/*!dir)*/dir == INVERT_Y_DIR) PORTF |= 128; else PORTF &= ~128; break;  //7 -
	case 2: if (/*!dir)*/dir == INVERT_Z_DIR) PORTL |= 2; else PORTL &= ~2; break;		//1 -
	case 3: if (dir == INVERT_E0_DIR) PORTA |= 64; else PORTA &= ~64; break;		//6 +
#endif
	}
	asm("nop");
}

uint8_t sm4_get_dir_bits(void)
{
    register uint8_t dir_bits = 0;
    register uint8_t portL = PORTL;
	//TODO -optimize in asm
#if ((MOTHERBOARD == BOARD_RAMBO_MINI_1_0) || (MOTHERBOARD == BOARD_RAMBO_MINI_1_3))
	if (portL & 2) dir_bits |= 1;
	if (portL & 1) dir_bits |= 2;
	if (portL & 4) dir_bits |= 4;
	if (portL & 64) dir_bits |= 8;
	dir_bits ^= dir_mask; //0x07; //invert XYZ, do not invert E
#elif ((MOTHERBOARD == BOARD_EINSY_1_0a))
	if (portL & 1) dir_bits |= 1;
	if (portL & 2) dir_bits |= 2;
	if (portL & 4) dir_bits |= 4;
	if (portL & 64) dir_bits |= 8;
	dir_bits ^= dir_mask; //0x0a; //invert YE, do not invert XZ
/*RAMPS*/
#elif ((MOTHERBOARD == BOARD_RAMPS_14_EFB))
	register uint8_t portF = PORTF;
	register uint8_t portA = PORTA;

	if (portF & 2) dir_bits |= 1;	//1 -
	if (portF & 128) dir_bits |= 2;	//7 -
	if (portL & 2) dir_bits |= 4;	//1 -
	if (portA & 64) dir_bits |= 8;	//6 +
	dir_bits ^= dir_mask; //0x07; //invert XYZ, do not invert E
#endif
	return dir_bits;
}

void sm4_set_dir_bits(uint8_t dir_bits)
{
    register uint8_t portL = PORTL;
	portL &= 0xb8; //set direction bits to zero
	//TODO -optimize in asm
#if ((MOTHERBOARD == BOARD_RAMBO_MINI_1_0) || (MOTHERBOARD == BOARD_RAMBO_MINI_1_3))
	dir_bits ^= dir_mask; //0x07; //invert XYZ, do not invert E
	if (dir_bits & 1) portL |= 2;  //set X direction bit
	if (dir_bits & 2) portL |= 1;  //set Y direction bit
	if (dir_bits & 4) portL |= 4;  //set Z direction bit
	if (dir_bits & 8) portL |= 64; //set E direction bit
#elif ((MOTHERBOARD == BOARD_EINSY_1_0a))
	dir_bits ^= dir_mask; //0x0a; //invert YE, do not invert XZ
	if (dir_bits & 1) portL |= 1;  //set X direction bit
	if (dir_bits & 2) portL |= 2;  //set Y direction bit
	if (dir_bits & 4) portL |= 4;  //set Z direction bit
	if (dir_bits & 8) portL |= 64; //set E direction bit
/*RAMPS*/
#elif (MOTHERBOARD == BOARD_RAMPS_14_EFB)
	register uint8_t portF = PORTF;
	register uint8_t portA = PORTA;

	dir_bits ^= dir_mask; //0x07; //invert XYZ, do not invert E
	portF &= 0x7d; //set direction bits to zero
	portL &= 0xfd; //set direction bits to zero
	portA &= 0xbf; //set direction bits to zero

	if (dir_bits & 1) portF |= 2;   //set X direction bit
	if (dir_bits & 2) portF |= 128; //set Y direction bit
	if (dir_bits & 4) portL |= 2;   //set Z direction bit
	if (dir_bits & 8) portA |= 64;  //set E direction bit

	PORTF = portF;
	asm("nop");
	PORTA = portA;
	asm("nop");
#endif
	PORTL = portL;
	asm("nop");
}

void sm4_do_step(uint8_t axes_mask)
{
#if ((MOTHERBOARD == BOARD_RAMBO_MINI_1_0) || (MOTHERBOARD == BOARD_RAMBO_MINI_1_3) || (MOTHERBOARD == BOARD_EINSY_1_0a))
    register uint8_t portC = PORTC & 0xf0;
	PORTC = portC | (axes_mask & 0x0f); //set step signals by mask
	asm("nop");
	PORTC = portC; //set step signals to zero
	asm("nop");
/*RAMPS*/
#elif (MOTHERBOARD == BOARD_RAMPS_14_EFB)
	register uint8_t portF = PORTF & 0xbe;
	register uint8_t portL = PORTL & 0xf7;
	register uint8_t portA = PORTA & 0xef;

	if (axes_mask & 0x01) {
		PORTF = portF | 0x01; //set step signals by mask
		asm("nop");
		PORTF = portF; //set step signals to zero
		asm("nop");
	}

	if (axes_mask & 0x02) {
		PORTF = portF | 0x40; //set step signals by mask
		asm("nop");
		PORTF = portF; //set step signals to zero
		asm("nop");
	}

	if (axes_mask & 0x04) {
		PORTL = portL | 0x08; //set step signals by mask
		asm("nop");
		PORTL = portL; //set step signals to zero
		asm("nop");
	}

	if (axes_mask & 0x08) {
		PORTA = portA | 0x10; //set step signals by mask
		asm("nop");
		PORTA = portA; //set step signals to zero
		asm("nop");
	}
#endif //((MOTHERBOARD == BOARD_RAMBO_MINI_1_0) || (MOTHERBOARD == BOARD_RAMBO_MINI_1_3) || (MOTHERBOARD == BOARD_EINSY_1_0a))
}

uint16_t sm4_line_xyze_ui(uint16_t dx, uint16_t dy, uint16_t dz, uint16_t de)
{
/*RAMPS*/
//let's adjust dx and dy for custom steps
#ifndef STEPS100  
  dx = dx * CUSTOM_X_STEPS / 100;
  dy = dy * CUSTOM_Y_STEPS / 100;
#endif
/*RAMPS*/
	uint16_t dd = (uint16_t)(sqrt((float)(((uint32_t)dx)*dx + ((uint32_t)dy*dy) + ((uint32_t)dz*dz) + ((uint32_t)de*de))) + 0.5);
	uint16_t nd = dd;
	uint16_t cx = dd;
	uint16_t cy = dd;
	uint16_t cz = dd;
	uint16_t ce = dd;
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t z = 0;
	uint16_t e = 0;
	while (nd)
	{
		if (sm4_stop_cb && (*sm4_stop_cb)()) break;
		uint8_t sm = 0; //step mask
		if (cx <= dx)
		{
			sm |= 1;
			cx += dd;
			x++;
		}
		if (cy <= dy)
		{
			sm |= 2;
			cy += dd;
			y++;
		}
		if (cz <= dz)
		{
			sm |= 4;
			cz += dd;
			z++;
		}
		if (ce <= de)
		{
			sm |= 4;
			ce += dd;
			e++;
		}
		cx -= dx;
		cy -= dy;
		cz -= dz;
		ce -= de;
		sm4_do_step(sm);
		uint16_t delay = SM4_DEFDELAY;
		if (sm4_calc_delay_cb) delay = (*sm4_calc_delay_cb)(nd, dd);
		if (delay) delayMicroseconds(delay);
		nd--;
	}
	if (sm4_update_pos_cb) (*sm4_update_pos_cb)(x, y, z, e);
	return nd;
}


#endif //NEW_XYZCAL
