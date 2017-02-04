//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"

#include "clocks.h"
#include "uart.hpp"
#include "ProtocolData.h"
#include "Protocol.hpp"
#include "TimerPWM_Servo.hpp"
#include "timer.hpp"
#include "Drivers.hpp"

void main(void) {

	WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer
	//<left right # # # # # # # # # # # # #>
	//Peripherials::UART uart(*EUSCI_A1, 9600, txQueue);

	P2->DIR |= 7;

	P3->SEL0 |= 0x0C;
	P3->DIR |= 0x08;

	P8->OUT |= 4;
	P8->DIR |= 4;
	P8->SEL0 |= 4;

	P10->SEL0 |= 1 << 5;
	P10->DIR |= 1 << 5;

	//EUSCI_A1->STATW |= UCLISTEN;

	NVIC_EnableIRQ(EUSCIA2_IRQn);
	NVIC_EnableIRQ(TA0_0_IRQn);
	NVIC_EnableIRQ(TA0_N_IRQn);
	NVIC_EnableIRQ(TA1_0_IRQn);
	NVIC_EnableIRQ(TA1_N_IRQn);
	NVIC_EnableIRQ(TA2_0_IRQn);
	NVIC_EnableIRQ(TA2_N_IRQn);
	NVIC_EnableIRQ(TA3_0_IRQn);
	NVIC_EnableIRQ(TA3_N_IRQn);

	Left.Center();
	Right.Center();

	ArmUpper.Center();
	ArmUpper.Center();

	Left.Resume();
	Right.Resume();
	ArmUpper.Resume();
	ArmUpper.Resume();

	int last_time = Peripherials::GetTA3().GetOverflowCount();
	int current_time = Peripherials::GetTA3().GetOverflowCount();
	Peripherials::UART& RFD900 = Peripherials::UART_A2;

	unsigned char* uart_data = RFD900.GetBuffer().GetData();

	while (1) {
		////////////////////////////////////////////////////////
		do {
			current_time = Peripherials::GetTA3().GetOverflowCount();
		} while ((current_time - last_time) == 0);
		last_time = current_time;
		////////////////////////////////////////////////////////

		int new_time = Peripherials::GetTA3().GetOverflowCount();
//		if (new_time - last_time > 50 * 1) {
//			Left.Center(127);
//			Right.Center(127);
//		}
		if (uart_data[0] == '<') {
			if (RFD900.GetBufferLength() >= 4) {
				last_time = Peripherials::GetTA3().GetOverflowCount();
				RFD900.ClearBuffer();
				Left.GoTo(uart_data[1]);
				Right.GoTo(uart_data[2]);
				uart_data[0] = '\0';
			}
		} else {
			RFD900.ClearBuffer();
		}

		Left.Tick(0.020f);
		Right.Tick(0.020f);
		ArmUpper.Tick(0.020f);
		ArmLower.Tick(0.020f);
	}
}

