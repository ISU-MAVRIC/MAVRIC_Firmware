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
#include "I2C.hpp"

#include <stdio.h>
float rate = 0;

void HandleU3() {
	int currentTime = Peripherials::GetTA3().GetOverflowCount();
	static int lastTime = currentTime - 500;
	static int interArrivalTimes[20];
	static int writeIndex = 0;
	static bool full = false;

	int delta = currentTime - lastTime;
	interArrivalTimes[writeIndex] = delta;
	if (!full && writeIndex == 20) {
		full = true;
	}
	writeIndex = (writeIndex + 1) % 20;

	int limit = 20;
	if (!full) {
		limit = writeIndex;
	}

	int total = 0;
	for (int i = 0; i < limit; i++) {
		total += interArrivalTimes[i];
	}

	rate = 50 / total; // Average hits per second

	lastTime = currentTime;
}

void main(void) {

	WDTCTL = WDTPW | WDTHOLD;

	P6->DIR |= 1 << 6;
	P6->OUT |= 1 << 6;
	P6->OUT &= ~(1 << 6);
	// Stop watchdog tim2
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

	U3Handler = HandleU3;

	InitializePeripherialPorts();
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

//	ArmUpper.GoTo(0.5);
//	ArmUpper.Resume();
//	while (1)
//	{
//		ArmUpper.Tick(0);
//	}

//	{
//		ClawPitch.Resume();
//		volatile int val = 127;
//		while (1) {
//			ClawPitch.GoTo(val);
//		}
//	}

//	{
//		ClawRotation.Resume();
//		volatile int val = 127;
//		while (1) {
//			ClawRotation.GoTo(val);
//		}
//	}

//	Claw.GoTo(0.5);
//	Claw.Resume();
//	while (1)
//	{
//		Claw.Tick(0.020);
//	}

	int last_time = Peripherials::GetTA3().GetOverflowCount();
	int current_time = last_time;
	int message_time = current_time - 600;
	Peripherials::UART& RFD900 = Peripherials::UART_A2;
	unsigned char* uart_data = RFD900.GetBuffer().GetData();
	bool suspended = true;

//	bool automation = 1;
//	if (automation)
//	{
////		__delay_cycles(3000000);
////		WDTCTL = WDTPW | WDTSSEL__ACLK | WDTIS_3 | WDTCNTCL;
//		InitI2C(EUSCI_B1);
//		Right.Resume();
//		Left.Resume();
//		unsigned char packet[4] = {0, 0, 0, 0};
//		while (1)
//		{
//			ReadI2C(EUSCI_B1, 13, packet, 4, 1);
////			Right.GoTo(packet[1]);
////			Left.GoTo(packet[2]);
////			WDTCTL |= WDTCNTCL;
//		}
//	}
//	InitI2C(EUSCI_B1);
	while (1) {
		current_time = Peripherials::GetTA3().GetOverflowCount();
		////////////////////////////////////////////////////////

		// 50 ticks = 1 second
		if (current_time - message_time > 50 * 1) {
			Left.Suspend();
			Right.Suspend();
			ArmUpper.Suspend();
			ArmLower.Suspend();
			ArmBase.Suspend();
			ClawPan.Suspend();
			ClawPitch.Suspend();
			CameraPitch.Suspend();
			CameraPan.Suspend();
			Claw.Suspend();
			ClawRotation.Suspend();
			SS_Arm.Suspend();
			SS_Depth.Suspend();
			SS_Drill.Suspend();
			suspended = true;
		}

		if (uart_data[0] == '<') {
			if (RFD900.GetBufferLength() >= 15) {
				message_time = Peripherials::GetTA3().GetOverflowCount();
				unsigned char data[16];
				memcpy(data, RFD900.GetBuffer().GetData(), sizeof(data));
				RFD900.ClearBuffer();
				float olower = (data[3] - 127) / 127.0f;
				float oupper = (data[4] - 127) / 127.0f;

//				printf("%c%3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d%c\n", data[0],
//						data[1], data[2], data[3], data[4], data[5], data[6],
//						data[7], data[8], data[9], data[10], data[11]);

				Right.GoTo(data[1]);
				Left.GoTo(data[2]);
				ClawPan.GoTo(data[5]);
				ClawPitch.GoTo(data[6]);
				ArmLower.GoTo(olower);
				ArmUpper.GoTo(oupper);
				ArmBase.GoTo((data[14] - 127) / 127.0f);
				CameraPan.GoTo(data[7]);
				CameraPitch.GoTo(data[8]);
				ClawRotation.GoTo(data[9]);
				Claw.GoTo(data[10]/255.0f);
				SS_Arm.GoTo((data[11] - 127) / 127.0f);
				SS_Depth.GoTo((data[12] - 127) / 127.0f);
				SS_Drill.GoTo((data[13] - 127) / 127.0f);

				if (suspended) {
					Right.Resume();
					Left.Resume();
					ClawPan.Resume();
					ClawPitch.Resume();
					ArmUpper.Resume();
					ArmLower.Resume();
					ArmBase.Resume();
					CameraPitch.Resume();
					CameraPan.Resume();
					Claw.Resume();
					ClawRotation.Resume();
					SS_Arm.Resume();
					SS_Depth.Resume();
					SS_Drill.Resume();
					suspended = false;
				}
				uart_data[0] = '\0';
			}
		} else {
			RFD900.ClearBuffer();
			uart_data[0] = '\0';
		}

		//testCameraStep();

		if (!suspended)
		{
			Claw.Tick((current_time - last_time) * 0.020f);
		}

		if (last_time != current_time) {
			// SS is in channel 2 (see ADC_Init)
			int SS_temp_val = MAP_ADC14_getResult(1 << 2);
			//printf("%d\n", SS_temp_val);
			unsigned char SS_MoisturePacket[2] = {0, 0};
//			WriteI2C(EUSCI_B1, 0x0D, SS_MoisturePacket, 1, 0);
//			ReadI2C(EUSCI_B1, 0x0D, SS_MoisturePacket, 5, 1);
			__no_operation();
		}

		last_time = current_time;
	}
}

void testCameraStep() {
//	static float pan = 127;
	static float pitch = 127;
//	static int panInc = 1;
	static int pitchInc = 2;
//	CameraPan.GoTo(pan);
	CameraPitch.GoTo(pitch);
//	if (pan + panInc >= 255 || pan + panInc <= 0) {
//		panInc *= -1;
//	}
//
	if (pitch + pitchInc >= 255 || pitch + pitchInc <= 0) {
		pitchInc *= -1;
	}
//
//	pan += panInc;
	pitch += pitchInc;

	static bool panSide = false;
	static int panCounter = 0;
	if (panCounter++ >= 150) {
		panCounter = 0;
		if (panSide) {
			CameraPan.GoTo(255);
		} else {
			CameraPan.GoTo(0);
		}
		panSide = !panSide;
	}
	__delay_cycles(fMCLK / 50);
}
