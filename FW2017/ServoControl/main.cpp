//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"

#include "timer.hpp"

using namespace Peripherials;

void main(void) {

	WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

	P2->DIR = 0x07 | (1 << 4);
	P2->OUT = 0x07;
	P2->SEL0 = 1 << 4;

	TA0.SetPeriod((float)20.0 / 1000);
	TA0.StartPWM(CC1, (float)1.5/1000);
	TA0.SetPWM(CC1, (float)1/1000);
	TA0.SetPWM(CC1, (float)2/1000);

	while (true) {
		__delay_cycles(3000000);
		P2->OUT = 0x00;
		__delay_cycles(50000000);
		P2->OUT = 0x07;
	}
}
