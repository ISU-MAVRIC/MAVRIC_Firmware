//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"

#include "timer.hpp"

using namespace Peripherials;

void ClearRed() {
	P2->OUT &= ~1;
}

void SetRed() {
	P2->OUT |= 1;
}

void ToggleGreen() {
	P2->OUT ^= 2;
}

void main(void) {

	WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

	P2->DIR = 0xFF;
	P2->SEL0 = 1 << 4;
	P2->OUT = 0;
	TA0.AttachOverflowInterrupt(ToggleGreen);
	TA0.AttachInterrupt(CC0, SetRed);
	TA0.AttachInterrupt(CC1, ClearRed);
	__enable_interrupt();
	TA0.SetPeriod((uint16_t)0xFFFF);
	TA0.StartPWM(CC1, (uint16_t)0x7FFF);
	/*
	uint16_t ledTime;
	while (true) {
		for (ledTime = 0; ledTime < 0xFFFF; ledTime += 0x1000) {
			TA0.SetPWM(CC1, (uint16_t)ledTime);
			__delay_cycles(4800000);
		}
	for (ledTime = 0xFFFF; ledTime > 0x0000; ledTime -= 0x1000) {
		TA0.SetPWM(CC1, (uint16_t)ledTime);
		__delay_cycles(4800000);
	}
	*/
	while (true);
}
