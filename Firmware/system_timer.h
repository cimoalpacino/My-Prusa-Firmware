//! @file

#ifndef FIRMWARE_SYSTEM_TIMER_H_
#define FIRMWARE_SYSTEM_TIMER_H_

#include "Arduino.h"

/*RAMPS*/
#ifdef ENABLE_SYSTEM_TIMER_2
#define SYSTEM_TIMER_2
#endif

#ifdef SYSTEM_TIMER_2
#include "timer02.h"
#define _millis millis2
#define _micros micros2
#define _delay delay2
#define _tone tone2
#define _noTone noTone2

#define timer02_set_pwm0(pwm0)

#else //SYSTEM_TIMER_2
#define _millis millis
#define _micros micros
#define _delay delay
#define _tone(x, y) /*tone*/
#define _noTone(x) /*noTone*/
#define timer02_set_pwm0(pwm0)
#endif //SYSTEM_TIMER_2

#endif /* FIRMWARE_SYSTEM_TIMER_H_ */
