//timer02.c
// use atmega timer2 as main system timer instead of timer0
// timer0 is used for fast pwm (OC0B output)
// original OVF handler is disabled

#if ((MOTHERBOARD == BOARD_RAMPS_14_EFB) && defined(ENABLE_SYSTEM_TIMER_2))
#include "system_timer.h"
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io_atmega2560.h"

/*RAMPS*/
#if MOTHERBOARD == BOARD_RAMPS_14_EFB
	#define BEEPER              33
	#include "Arduino.h" //otherwise clockCyclesPerMicrosecond is not defined!
#else
	#define BEEPER              84
#endif

#if ((MOTHERBOARD == BOARD_RAMPS_14_EFB) && !defined(SYSTEM_TIMER_2))
uint8_t timer02_pwm0 = 0;

void timer02_set_pwm0(uint8_t pwm0)
{
	if (timer02_pwm0 == pwm0) return;
	if (pwm0)
	{
		TCCR0A |= (2 << COM0B0);
		OCR0B = pwm0 - 1;
	}
	else
	{
		TCCR0A &= ~(2 << COM0B0);
		OCR0B = 0;
	}
	timer02_pwm0 = pwm0;
}

#elif (MOTHERBOARD == BOARD_RAMPS_14_EFB) && defined(SYSTEM_TIMER_2)
void timer4_init(void)
{
	//save sreg
	uint8_t _sreg = SREG;
	//disable interrupts for sure
	cli();

	TCNT4 = 0; //TCNTH = 0; //TCNTL = 0:
	// Fast PWM duty (0-255). 
	// Due to invert mode (following rows) the duty is set to 255, which means zero all the time (bed not heating)
	OCR4C = 255; //OCR4CH = 0; //OCR4CL = 255:

	// Set fast PWM mode and inverting mode.
	// WGMx => Fast PWM/TOP=0x00FF/Update of OCRA at BOTTOM=0x0000/TOV Flag Set on TOP=0xFF
	// COM0Ax => 00=Normal port operation, OC0A disconnected
	// COM0Bx => 11=Set OR0B on Compare Match, clear OC0B at BOTTOM (inverting mode)
	TCCR4A = (1 << WGM40) | (1 << COM4C1) | (1 << COM4C0);
	//TCCR4B=ICNC4|ICES4|-|WGM43|WGM42|CS42|CS41|CS40
	TCCR4B = (1 << WGM42) | (1 << CS41) | (1 << CS40);    // CLK/64 prescaling
	//TCCR4B = (1 << WGM42) | (1 << CS40); //TEST   // CLK/1 prescaling
	TIMSK4 |= (1 << TOIE4);  // enable timer overflow interrupt

	// Everything, that used to be on timer4 was moved to timer0 (delay, beeping, millis etc.)
	 //setup timer0
	TCCR0A = 0; //COM_A-B=00, WGM_0-1=00
				/*RAMPS*/ //testiraj z 1,2 in 3
	TCCR0B = (1 << CS00); //no prescaling (clk/1)
	//mask timer0 interrupts - enable OVF, disable others
	TIMSK0 |= (1 << TOIE0);
	TIMSK0 &= ~(1 << OCIE0A);
	TIMSK0 &= ~(1 << OCIE0B);
	//set timer0 OCR registers (OCRB interrupt generated 0.5ms after OVF interrupt)
	OCR0A = 0;
	OCR0B = 128;
	//restore sreg (enable interrupts)
	SREG = _sreg;
}

// The following code is OVF handler for timer 2
// it was copy-pasted from wiring.c and modified for timer2
// variables timer0_overflow_count and timer0_millis are declared in wiring.c

// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)
#else
// Original Prusa code for Rambo/Einsy boards
void timer0_init(void)
{
	//save sreg
	uint8_t _sreg = SREG;
	//disable interrupts for sure
	cli();

	TCNT0 = 0;
	// Fast PWM duty (0-255). 
	// Due to invert mode (following rows) the duty is set to 255, which means zero all the time (bed not heating)
	OCR0B = 255;
	// Set fast PWM mode and inverting mode. 
	// Set OC0B on compare match, clear OC0B at BOTTOM (inverting mode)
	// 0xff / top / max=0xff
	TCCR0A = (1 << WGM01) | (1 << WGM00) | (1 << COM0B1) | (1 << COM0B0);
	TCCR0B = (1 << CS01);    // CLK/8 prescaling = 
	TIMSK0 |= (1 << TOIE0);  // enable timer overflow interrupt

							 // Everything, that used to be on timer0 was moved to timer2 (delay, beeping, millis etc.)
							 //setup timer2
	TCCR2A = 0x00; //COM_A-B=00, WGM_0-1=00
	TCCR2B = (1 << CS20); //WGM_2=0, CS_0-2=011
						  //mask timer2 interrupts - enable OVF, disable others
	TIMSK2 |= (1 << TOIE2);
	TIMSK2 &= ~(1 << OCIE2A);
	TIMSK2 &= ~(1 << OCIE2B);
	//set timer2 OCR registers (OCRB interrupt generated 0.5ms after OVF interrupt)
	OCR2A = 0;
	OCR2B = 128;
	//restore sreg (enable interrupts)
	SREG = _sreg;
}


// The following code is OVF handler for timer 2
// it was copy-pasted from wiring.c and modified for timer2
// variables timer0_overflow_count and timer0_millis are declared in wiring.c

// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

#endif



volatile unsigned long timer2_overflow_count;
volatile unsigned long timer2_millis;
unsigned char timer2_fract = 0;

#if ((MOTHERBOARD == BOARD_RAMPS_14_EFB) && defined(SYSTEM_TIMER_2))
ISR(TIMER0_OVF_vect)
{
	// copy these to local variables so they can be stored in registers
	// (volatile variables must be read from memory on every access)
	unsigned long m = timer2_millis;
	unsigned char f = timer2_fract;
	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX)
	{
		f -= FRACT_MAX;
		m += 1;
	}
	timer2_fract = f;
	timer2_millis = m;
	timer2_overflow_count++;
}
#elif (MOTHERBOARD != BOARD_RAMPS_14_EFB)
ISR(TIMER2_OVF_vect)
{
	// copy these to local variables so they can be stored in registers
	// (volatile variables must be read from memory on every access)
	unsigned long m = timer2_millis;
	unsigned char f = timer2_fract;
	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX)
	{
		f -= FRACT_MAX;
		m += 1;
	}
	timer2_fract = f;
	timer2_millis = m;
	timer2_overflow_count++;
}
#endif

unsigned long millis2(void)
{
	unsigned long m;
	uint8_t oldSREG = SREG;

	// disable interrupts while we read timer0_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer0_millis)
	cli();
	m = timer2_millis;
	SREG = oldSREG;

	return m;
}

unsigned long micros2(void)
{
#if (MOTHERBOARD == BOARD_RAMPS_14_EFB) && defined(SYSTEM_TIMER_2)
	unsigned long m;
	uint8_t oldSREG = SREG, t;
	cli();
	m = timer2_overflow_count;
#if defined(TCNT0)
	t = TCNT0;
#elif defined(TCNT0L)
	t = TCNT0L;
#else
#error TIMER 0 not defined
#endif
#ifdef TIFR0
	if ((TIFR0 & _BV(TOV0)) && (t < 255))
		m++;
#else
	if ((TIFR & _BV(TOV2)) && (t < 255))
		m++;
#endif
	SREG = oldSREG;
	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
#else
	unsigned long m;
	uint8_t oldSREG = SREG, t;
	cli();
	m = timer2_overflow_count;
#if defined(TCNT2)
	t = TCNT2;
#elif defined(TCNT2L)
	t = TCNT2L;
#else
	#error TIMER 2 not defined
#endif
#ifdef TIFR2
	if ((TIFR2 & _BV(TOV2)) && (t < 255))
		m++;
#else
	if ((TIFR & _BV(TOV2)) && (t < 255))
		m++;
#endif
	SREG = oldSREG;	
	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
#endif
}

void delay2(unsigned long ms)
{
	uint32_t start = micros2();
	while (ms > 0)
	{
		yield();
		while ( ms > 0 && (micros2() - start) >= 1000)
		{
			ms--;
			start += 1000;
		}
	}
}

void tone2(__attribute__((unused)) uint8_t _pin, __attribute__((unused)) unsigned int frequency/*, unsigned long duration*/)
{
	PIN_SET(BEEPER);
}

void noTone2(__attribute__((unused)) uint8_t _pin)
{
	PIN_CLR(BEEPER);
}

