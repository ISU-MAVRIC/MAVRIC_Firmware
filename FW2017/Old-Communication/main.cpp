//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"

#include <string.h>

#include "clocks.h"
#include "uart.hpp"
#include "ProtocolData.h"
#include "Protocol.hpp"
#include "TimerPWM_Servo.hpp"
#include "timer.hpp"
#include "Drivers.hpp"
#include <../../driverlib/driverlib/msp432p4xx/driverlib.h>

void main(void) {

	WDTCTL = WDTPW | WDTHOLD;
	// Stop watchdog timer
	//<left right # # # # # # # # # # # # #>
	//Peripherials::UART uart(*EUSCI_A1, 9600, txQueue);

	P2->DIR |= 7;

	P3->SEL0 |= 0x0C;
	P3->DIR |= 0x08;

	P8->DIR |= (1 << 2);
	P8->SEL0 |= (1 << 2);

	P10->SEL0 |= 1 << 5;
	P10->DIR |= 1 << 5;

	P5->DIR |= (1 << 7);
	P5->SEL0 |= (1 << 7);

	// Linear Actuator direction pins
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7);

	// TA2.3 (P6.6)
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P6, GPIO_PIN6,
	GPIO_PRIMARY_MODULE_FUNCTION);

	// TA0.1 (P2.4)
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,
	GPIO_PRIMARY_MODULE_FUNCTION);
	// TA0.2 (P2.5)
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN5,
	GPIO_PRIMARY_MODULE_FUNCTION);

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
	NVIC_EnableIRQ(ADC14_IRQn);

	Left.Center();
	Right.Center();

	ArmUpper.Center();
	ArmLower.GoTo(1);
	ClawPan.Center();
	ClawPitch.Center();

//	Left.Resume();
//	Right.Resume();
//	ArmUpper.Resume();
//	ArmLower.Resume();
//	ClawPan.Resume();
//	ClawPitch.Resume();

	int last_time = Peripherials::GetTA3().GetOverflowCount();
	int current_time = last_time;
	int message_time = current_time - 600;
	Peripherials::UART& RFD900 = Peripherials::UART_A2;
	unsigned char* uart_data = RFD900.GetBuffer().GetData();
	bool suspended = true;

	while (1) {
		current_time = Peripherials::GetTA3().GetOverflowCount();
		////////////////////////////////////////////////////////

		// 50 ticks = 1 second
		if (current_time - message_time > 50 * 1) {
			Left.Suspend();
			Right.Suspend();
			ArmUpper.Suspend();
			ArmLower.Suspend();
			ClawPan.Suspend();
			ClawPitch.Suspend();
			suspended = true;
		}

		if (uart_data[0] == '<') {
			if (RFD900.GetBufferLength() >= 8) {
				message_time = Peripherials::GetTA3().GetOverflowCount();
				unsigned char data[8];
				memcpy(data, RFD900.GetBuffer().GetData(), sizeof(data));
				RFD900.ClearBuffer();
				float olower = (data[3]) / 255.0f;
				float oupper = (data[4]) / 255.0f;

				Right.GoTo(data[1]);
				Left.GoTo(data[2]);
				ClawPan.GoTo(data[5]);
				ClawPitch.GoTo(data[6]);
				ArmLower.GoTo(olower);
				ArmUpper.GoTo(oupper);

				if (suspended) {
					Right.Resume();
					Left.Resume();
					ClawPan.Resume();
					ClawPitch.Resume();
					ArmUpper.Resume();
					ArmLower.Resume();
					suspended = false;
				}
//				Right.GoTo(uart_data[5]);
//				Right.GoTo(uart_data[6]);
				uart_data[0] = '\0';
			}
		} else {
			RFD900.ClearBuffer();
		}

		if (!suspended) {
			float delta = 0.020f * (current_time - last_time);
			Left.Tick(delta);
			Right.Tick(delta);
			ArmLower.Tick(delta);
			ArmUpper.Tick(delta);
		}

		last_time = current_time;
	}
}

