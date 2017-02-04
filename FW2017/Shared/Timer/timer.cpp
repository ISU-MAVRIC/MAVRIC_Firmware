/*
 * timer.cpp
 *
 *  Created on: Sep 3, 2016
 *      Author: James
 */

#include "timer.hpp"
#include "msp.h"
#include "clocks.h"

#include <string.h>

namespace Peripherials {

Timer& GetTA0()
{
	static Timer ta0(TIMER_A0, ACLK);
	return ta0;
}

Timer& GetTA1()
{
	static Timer ta1(TIMER_A1, ACLK);
	return ta1;
}

Timer& GetTA2()
{
	static Timer ta2(TIMER_A2, ACLK);
	return ta2;
}

Timer& GetTA3()
{
	static Timer ta3(TIMER_A3, ACLK);
	return ta3;
}

Timer &Timer::operator=(const Peripherials::Timer & other)
{
	regs = other.regs;
	timer_module = other.timer_module;
	memcpy(isr, other.isr, sizeof(isr));
	overflow_isr = other.overflow_isr;
	overflow_count = other.overflow_count;
	return *this;
}

Timer::Timer(Timer_A_Type* instance, TimerClockSource source) :
		regs(*instance), overflow_count(0) {

	regs.CTL = ((source & 3) << 8) | (TACLR) | (TIMER_A_CTL_MC__UP);

	if (instance == TIMER_A0) {
		timer_module = 0;
	} else if (instance == TIMER_A1) {
		timer_module = 1;
	} else if (instance == TIMER_A2) {
		timer_module = 2;
	} else if (instance == TIMER_A3) {
		timer_module = 3;
	}

	regs.CTL |= TAIE;
	regs.CCTL[0] |= TIMER_A_CCTLN_CCIE;

	overflow_isr = 0;
	int i;
	for (i = 0; i < sizeof(isr)/sizeof(*isr); i++) {
		isr[i] = 0;
	}
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
	int clock_divider_base = ((regs.CTL >> 6) & 0x03) + (regs.EX0 & 0x07);
	switch ((TimerClockSource) (regs.CTL >> 8) & 0x3) {
	/* umm, this is external without a fixed period, so do nothing */
	case TAxCLK:
	case INCLK:
		break;

	case ACLK:
		/* set the period to the computed value */
		regs.CCR[0] =
				(uint16_t) (((fACLK >> clock_divider_base) * period) + 0.5);
		/* Set the mode to up (to make use of the period) */
		regs.CTL = (regs.CTL & ~(3 << 4)) | (MC__UP);
		break;

	case SMCLK:
		/* set the period to the computed value */
		regs.CCR[0] = (uint16_t) (((fSMCLK >> clock_divider_base) * period)
				+ 0.5);
		/* Set the mode to up (to make use of the period) */
		regs.CTL = (regs.CTL & ~(3 << 4)) | (MC__UP);
		break;
	}
}

void Timer::StartPWM(TimerCapComUnit module, float pulse_width) {
	if (module == CC0)
		return;
	int clock_divider_base = ((regs.CTL >> 6) & 0x03) + (regs.EX0 & 0x07);

	switch ((TimerClockSource) (regs.CTL >> 8) & 0x3) {
	/* umm, this is external without a fixed period, so do nothing */
	case TAxCLK:
	case INCLK:
		break;

	case ACLK:
		regs.CCTL[(int) module] = (regs.CCTL[(int) module] & ~OUTMOD_7 & ~CAP)
				| OUTMOD_7;
		regs.CCR[(int) module] = (uint16_t) ((pulse_width
				* (fACLK >> clock_divider_base)) + 0.5);
		break;

	case SMCLK:
		regs.CCTL[(int) module] = (regs.CCTL[(int) module] & ~OUTMOD_7 & ~CAP)
				| OUTMOD_7;
		regs.CCR[(int) module] = (uint16_t) ((pulse_width
				* (fSMCLK >> clock_divider_base)) + 0.5);
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
		regs.CCTL[(int) module] = (regs.CCTL[(int) module] & ~OUTMOD_7 & ~CAP)
				| OUTMOD_7;
		regs.CCR[(int) module] = pulse_width;
		break;
	}
}

void Timer::SetPWM(TimerCapComUnit module, float pulse_width) {
	if (module == CC0)
		return;

	switch ((TimerClockSource) (regs.CTL >> 8) & 0x3) {
	/* umm, this is external without a fixed period, so do nothing */
	case TAxCLK:
	case INCLK:
		break;

	case ACLK:
		regs.CCR[(int) module] = (uint16_t) ((pulse_width * fACLK) + 0.5);
		break;

	case SMCLK:
		regs.CCR[(int) module] = (uint16_t) ((pulse_width * fSMCLK) + 0.5);
		break;
	}
}

void Timer::SetPWM(TimerCapComUnit module, uint16_t pulse_width) {
	switch ((TimerClockSource) (regs.CTL >> 8) & 0x3) {
	/* umm, this is external without a fixed period, so do nothing */
	case TAxCLK:
	case INCLK:
		break;

	case ACLK:
	case SMCLK:
		regs.CCR[(int) module] = pulse_width;
		break;
	}
}

void Timer::DisablePWM(TimerCapComUnit module) {
	regs.CCTL[(int) module] = (regs.CCTL[(int) module] & ~OUTMOD_7) | OUTMOD_5;
}

void Timer::EnablePWM(TimerCapComUnit module) {
	regs.CCTL[(int) module] = (regs.CCTL[(int) module] & ~OUTMOD_7 & ~CAP
			& ~CM_3) | OUTMOD_7;
}

void Timer::AttachOverflowInterrupt(void (*new_interrupt)(void)) {
	overflow_isr = new_interrupt;
	if (new_interrupt != 0) {
		regs.CTL |= TAIE;
		NVIC_EnableIRQ((IRQn_Type) (TA0_0_IRQn + (timer_module * 2)));
	} else {
		regs.CTL &= ~TAIE;
	}
}

void Timer::AttachInterrupt(TimerCapComUnit module,
		void (*new_interrupt)(void)) {
	isr[(int) module] = new_interrupt;

	if (new_interrupt != 0) {
		regs.CCTL[(int) module] |= CCIE;
		if (module == CC0) {
			NVIC_EnableIRQ((IRQn_Type) (TA0_0_IRQn + (timer_module * 2)));
		} else {
			NVIC_EnableIRQ((IRQn_Type) (TA0_N_IRQn + (timer_module * 2)));
		}
	} else {
		regs.CCTL[(int) module] &= ~CCIE;
	}
}

unsigned int Timer::GetOverflowCount() {
	return overflow_count;
}

void Timer::_CCR0_ISR(void) {
	if ((regs.CTL & TAIE) && (regs.CTL & TAIFG)) {
		if (overflow_isr != 0) {
			overflow_isr();
		}
		overflow_count++;
		regs.CTL &= ~TAIFG;
	}
	if ((regs.CCTL[0] & CCIE) && (regs.CCTL[0] & CCIFG)) {
		if (isr[0] != 0) {
			isr[0]();
		}
		regs.CCTL[0] &= ~CCIFG;
	}
}

void Timer::_CCRn_ISR(void) {
	int n = regs.IV / 2;
	if (n == 7) {
		if (overflow_isr != 0) {
			overflow_isr();
		}
		overflow_count++;
	}
	if (n <= 0 || n > 4) {
		return;
	}
	if (isr[n] != 0) {
		isr[n]();
	}
}
}

extern "C" {
void TA0_0_IRQHandler() {
	Peripherials::GetTA0()._CCR0_ISR();
}
void TA0_N_IRQHandler() {
	Peripherials::GetTA0()._CCRn_ISR();
}
/////////////////////////
void TA1_0_IRQHandler() {
	Peripherials::GetTA1()._CCR0_ISR();
}
void TA1_N_IRQHandler() {
	Peripherials::GetTA1()._CCRn_ISR();
}
/////////////////////////
void TA2_0_IRQHandler() {
	Peripherials::GetTA2()._CCR0_ISR();
}
void TA2_N_IRQHandler() {
	Peripherials::GetTA2()._CCRn_ISR();
}
/////////////////////////
void TA3_0_IRQHandler() {
	Peripherials::GetTA3()._CCR0_ISR();
}
void TA3_N_IRQHandler() {
	Peripherials::GetTA3()._CCRn_ISR();
}
}
