/*
 * Timer.hpp
 *
 *  Created on: Sep 3, 2016
 *      Author: James
 */

#ifndef TIMER_TIMER_HPP_
#define TIMER_TIMER_HPP_

#include "msp.h"
#include <stdint.h>

namespace Peripherials {

typedef enum TimerClockSource {
	TAxCLK = 0, ACLK = 1, SMCLK = 2, INCLK = 3,
} TimerClockSource;

typedef enum TimerCapComUnit {
	CC0, CC1, CC2, CC3
} TimerCapComUnit;

class Timer {
private:
	Timer_A_Type& regs;
	void (*isr[5])(void);
	void (*overflow_isr)(void);

public:
	/**
	 * instance is one of TIMER_A0 - TIMER_A3
	 */
	Timer(Timer_A_Type* instance, TimerClockSource source);

	void SetPeriod(uint16_t period);
	void SetPeriod(float period);

	/**
	 * The mapped pin must be set to output by the client.
	 */
	void StartPWM(TimerCapComUnit module, float pulse_width);
	void StartPWM(TimerCapComUnit module, uint16_t pulse_width);

	void SetPWM(TimerCapComUnit module, float pulse_width);
	void SetPWM(TimerCapComUnit module, uint16_t pulse_width);

	/**
	 * Calls the interrupt on a timer reset (PWM mode),
	 * a timer match (timer mode),
	 * or a input event (capture mode).
	 */
	void AttachOverflowInterrupt(void (*new_interrupt)(void));
	void AttachInterrupt(TimerCapComUnit module, void (*new_interrupt)(void));

	/**
	 * Don't call this, used exclusively by library code
	 */
	void _CCR0_ISR(void);
	/**
	 * Don't call this, used exclusively by library code
	 */
	void _CCRn_ISR(void);
};

extern Timer TA0, TA1, TA2, TA3;
}

#endif /* TIMER_TIMER_HPP_ */
