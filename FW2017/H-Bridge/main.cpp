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
#include "H_Bridge.hpp"

using namespace Peripherials;

void main(void) {

	WDTCTL = WDTPW | WDTHOLD;
	// Stop watchdog timer
	//<left right # # # # # # # # # # # # #>
	//Peripherials::UART uart(*EUSCI_A1, 9600, txQueue);

	// Linear Actuator direction pins
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7);

	// TA2.2 (H-Bridge 1 PWM, Channel A)
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN7,
	GPIO_PRIMARY_MODULE_FUNCTION);

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

	struct internal_servo_cal_point low = { 0, 0.000 };
	struct internal_servo_cal_point high = { 1, 0.021 };
	PinID dir_pin = { 1, 6 };
	H_Bridge& A = *new H_Bridge(
			*new InternalServoControl(GetTA2(), CC2, low, high), dir_pin);

	A.Resume();
	float amt = 0;
	while (1) {
		while (amt < 1) {
			A.GoTo(amt);
			amt += 0.1;
			__delay_cycles(1000000);
		}
		while (amt > -1) {
			A.GoTo(amt);
			amt -= 0.1;
			__delay_cycles(1000000);
		}
	}

}

