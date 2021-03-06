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
#include "PCA9685.hpp"

void main(void) {

	WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

	//Peripherials::UART uart(*EUSCI_A1, 9600, txQueue);

	P2->DIR = 1 << 3;
	P2->SEL0 = 1 << 3;

	P9->DIR = 1 << 7;
	P9->SEL0 = (1 << 7) | (1 << 6);

	P1->SEL0 = (1 << 6) | (1 << 7);
	P1->SEL0 = (1 << 6) | (1 << 7);

	P2->DIR |= 7;

	//EUSCI_A1->STATW |= UCLISTEN;

	NVIC_EnableIRQ(EUSCIA3_IRQn);
	NVIC_EnableIRQ(EUSCIA1_IRQn);

	Incoming packet;
	packet.header.raw = 0;
	packet.header.ding = 1;

	Externals::pca9685_servo_cal_point low = { -90, 100 };
	Externals::pca9685_servo_cal_point high = { 90, 500 };

	Externals::PCA9685 driver(EUSCI_B0, 0x40);
	driver.Port0.SetCalibration(low, high);
	driver.Port1.SetCalibration(low, high);

	driver.Port0.GoTo(45);
	driver.Port1.GoTo(45);

	driver.Port0.Resume();
	driver.Port1.Resume();

	__delay_cycles(fMCLK / 2);

	driver.Port0.GoTo(-45);
	driver.Port1.GoTo(-45);

	__delay_cycles(fMCLK / 2);

	driver.Port0.Suspend();
	driver.Port1.Suspend();

//	while (1)
//	{
//		Peripherials::UART_A3.Send(0x55);
//	}

	while (1) {

		while (Peripherials::UART_A3.GetBufferLength() <= 0)
			;
		packet.header.raw = Peripherials::UART_A3.GetBuffer().GetData()[0];
		while (!IsComplete(packet.header,
				Peripherials::UART_A3.GetBufferLength() - 1))
			;
		packet = Decode(Peripherials::UART_A3.GetBuffer().GetData());

		if (packet.header.wheels)
		{
			driver.Port0.GoTo(packet.command.lWheel);
			driver.Port1.GoTo(packet.command.rWheel);
		}

		if (packet.header.ding) {
			P2->OUT |= 7;
		} else {
			P2->OUT &= ~7;
		}

		Peripherials::UART_A3.ClearBuffer();
	}
}
