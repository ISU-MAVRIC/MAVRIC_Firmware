/*
 * timer.cpp
 *
 *  Created on: Sep 3, 2016
 *      Author: James
 */

#include "timer.hpp"
#include "msp.h"
#include "clocks.h"

namespace Peripherials {
Timer TA0(TIMER_A0, ACLK), TA1(TIMER_A1, ACLK), TA2(TIMER_A2, ACLK), TA3(
TIMER_A3, ACLK);

Timer::Timer(Timer_A_Type* instance, TimerClockSource source) :
		regs(*instance) {
	regs.CTL = (source & (3 << 8)) | (TACLR);
}

void Timer::SetPeriod(uint16_t period) {
	switch ((TimerClockSource) (regs.CTL >> 8) & 0x3) {
	/* umm, this is external without a fixed period, so do nothing */
	case TAxCLK:
	case INCLK:
		break;

	case ACLK:
	case SMCLK:
		/* set the period to the provided value */
		regs.CCR[0] = period;
		/* Set the mode to up (to make use of the period) */
		regs.CTL = (regs.CTL & ~(3 << 4)) | (MC__UP);
		break;
	}
}

void Timer::SetPeriod(float period) {
	switch ((TimerClockSource) (regs.CTL >> 8) & 0x3) {
	/* umm, this is external without a fixed period, so do nothing */
	case TAxCLK:
	case INCLK:
		break;

	case ACLK:
		/* set the period to the computed value */
		regs.CCR[0] = (uint16_t) ((ACLK * period) + 0.5);
		/* Set the mode to up (to make use of the period) */
		regs.CTL = (regs.CTL & ~(3 << 4)) | (MC__UP);
		break;

	case SMCLK:
		/* set the period to the computed value */
		regs.CCR[0] = (uint16_t) ((SMCLK * period) + 0.5);
		/* Set the mode to up (to make use of the period) */
		regs.CTL = (regs.CTL & ~(3 << 4)) | (MC__UP);
		break;
	}
}

void Timer::StartPWM(TimerCapComUnit module, float pulse_width) {
	switch ((TimerClockSource) (regs.CTL >> 8) & 0x3) {
	/* umm, this is external without a fixed period, so do nothing */
	case TAxCLK:
	case INCLK:
		break;

	case ACLK:
		regs.CCTL[(int) module] = CAP | OUTMOD_7;
		regs.CCR[(int) module] = (uint16_t) ((pulse_width * ACLK) + 0.5);
		break;

	case SMCLK:
		regs.CCTL[(int) module] = CAP | OUTMOD_7;
		regs.CCR[(int) module] = (uint16_t) ((pulse_width * SMCLK) + 0.5);
		break;
	}
}

void Timer::StartPWM(TimerCapComUnit module, uint16_t pulse_width) {
	switch ((TimerClockSource) (regs.CTL >> 8) & 0x3) {
	/* umm, this is external without a fixed period, so do nothing */
	case TAxCLK:
	case INCLK:
		break;

	case ACLK:
	case SMCLK:
		regs.CCTL[(int) module] = CAP | OUTMOD_7;
		regs.CCR[(int) module] = pulse_width;
		break;
	}
}

void Timer::AttachOverflowInterrupt(void (*new_interrupt)(void)) {
	overflow_isr = new_interrupt;
	if (new_interrupt != 0) {
		regs.CTL |= TAIE;
	} else {
		regs.CTL &= ~TAIE;
	}
}

void Timer::AttachInterrupt(TimerCapComUnit module,
		void (*new_interrupt)(void)) {
	isr[(int) module] = new_interrupt;

	if (new_interrupt != 0) {
		regs.CCTL[(int) module] |= CCIE;
	} else {
		regs.CCTL[(int) module] &= ~CCIE;
	}
}

void Timer::_CCR0_ISR(void) {
	if (isr[0] != 0) {
		isr[0]();
	}
}

void Timer::_CCRn_ISR(void) {
	int n = regs.IV / 2;
	if (isr[n] != 0) {
		isr[n]();
	}
}

namespace {

void TA0_0_IRQHandler() {
	TA0._CCR0_ISR();
}
void TA0_N_IRQHandler() {
	TA0._CCRn_ISR();
}
/////////////////////////
void TA1_0_IRQHandler() {
	TA1._CCR0_ISR();
}
void TA1_N_IRQHandler() {
	TA1._CCRn_ISR();
}
/////////////////////////
void TA2_0_IRQHandler() {
	TA2._CCR0_ISR();
}
void TA2_N_IRQHandler() {
	TA2._CCRn_ISR();
}
/////////////////////////
void TA3_0_IRQHandler() {
	TA3._CCR0_ISR();
}
void TA3_N_IRQHandler() {
	TA3._CCRn_ISR();
}
}
}
