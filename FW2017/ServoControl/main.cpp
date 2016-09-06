//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"

#include "timer.hpp"
#include "servo.hpp"

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
#define scale 1000

void main(void) {

	WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

	P2->DIR = 0xFF;
	P2->SEL0 = 1 << 4;
	P2->OUT = 0;
	TA0.AttachOverflowInterrupt(ToggleGreen);
	TA0.AttachInterrupt(CC0, ClearRed);
	TA0.AttachInterrupt(CC1, SetRed);
	__enable_interrupt();
	TA0.SetPeriod((float) (20.0 / scale));
	TA0.StartPWM(CC1, (float) (1.5 / scale));

	float PWM_time = 0;
	while (true) {
		for (PWM_time = 0; PWM_time < 20; PWM_time += 0.1) {
			TA0.SetPWM(CC1, (PWM_time / scale));
			__delay_cycles(480000);
		}
		for (PWM_time = 20; PWM_time > 0; PWM_time -= 0.1) {
			TA0.SetPWM(CC1, (PWM_time / scale));
			__delay_cycles(480000);
		}
	}

	while (true)
		;
}
