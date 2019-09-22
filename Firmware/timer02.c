//timer02.h
// use atmega timer2 as main system timer instead of timer0
// timer0 is used for fast pwm (OC0B output)
// original OVF handler is disabled
#ifndef TIMER02_H
#define TIMER02_H

#include <inttypes.h>

#if defined(__cplusplus)
extern "C" {
#endif //defined(__cplusplus)

/*RAMPS*/
#if (MOTHERBOARD == BOARD_RAMPS_14_EFB) && defined(SYSTEM_TIMER_2)
///! Initializes TIMER4 for fast PWM mode-driven bed heating
	extern void timer4_init(void);
#elif (MOTHERBOARD == BOARD_RAMPS_14_EFB) && !defined(SYSTEM_TIMER_2)
extern uint8_t timer02_pwm0;
extern void timer02_set_pwm0(uint8_t pwm0);
extern void timer02_init(void);
#else
	///! Initializes TIMER0 for fast PWM mode-driven bed heating
	extern void timer0_init(void);
#endif // MOTHERBOARD == BOARD_RAMPS_14_EFB

///! Reimplemented original millis() using timer2
extern unsigned long millis2(void);

///! Reimplemented original micros() using timer2
extern unsigned long micros2(void);

///! Reimplemented original delay() using timer2
extern void delay2(unsigned long ms);

///! Reimplemented original tone() using timer2
///! Does not perform any PWM tone generation, it just sets the beeper pin to 1
extern void tone2(uint8_t _pin, unsigned int frequency/*, unsigned long duration*/);

///! Turn off beeping - set beeper pin to 0
extern void noTone2(uint8_t _pin);

#if defined(__cplusplus)
}
#endif //defined(__cplusplus)

#endif //TIMER02_H
