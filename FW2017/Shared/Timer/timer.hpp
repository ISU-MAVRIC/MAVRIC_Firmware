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

#define _ms * (1.0 / 1000)

namespace Peripherials {

typedef enum TimerClockSource {
	TAxCLK = 0, ACLK = 1, SMCLK = 2, INCLK = 3,
} TimerClockSource;

typedef enum TimerCapComUnit {
	CC0, CC1, CC2, CC3, CC4
} TimerCapComUnit;

class Timer {
private:
	Timer_A_Type& regs;
	int timer_module;
	void (*isr[5])(void);
	void (*overflow_isr)(void);
	unsigned int overflow_count;

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

	void DisablePWM(TimerCapComUnit module);
	void EnablePWM(TimerCapComUnit module);

	/**
	 * Calls the interrupt on a timer reset (PWM mode)
	 */
	void AttachOverflowInterrupt(void (*new_interrupt)(void));
	void AttachInterrupt(TimerCapComUnit module, void (*new_interrupt)(void));

	unsigned int GetOverflowCount();

	/**
	 * Don't call this, used exclusively by library code
	 */
	void _CCR0_ISR(void);
	/**
	 * Don't call this, used exclusively by library code
	 */
	void _CCRn_ISR(void);

	Timer &operator=(const Peripherials::Timer &);
};

Timer& GetTA0();
Timer& GetTA1();
Timer& GetTA2();
Timer& GetTA3();

void InitTimers();
}

extern "C"
{
void TA0_0_IRQHandler();
void TA0_N_IRQHandler();
////////////////////////
void TA1_0_IRQHandler();
void TA1_N_IRQHandler();
////////////////////////
void TA2_0_IRQHandler();
void TA2_N_IRQHandler();
////////////////////////
void TA3_0_IRQHandler();
void TA3_N_IRQHandler();
}

#endif /* TIMER_TIMER_HPP_ */
