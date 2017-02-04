/*
 * LinearActuator.cpp
 *
 *  Created on: Feb 4, 2017
 *      Author: Talonj123
 */

#include "LinearActuator.hpp"
#include "msp.h"

int InitADC14() {
	static int Actuators = 0;
	static bool inited = false;
	if (!inited) {
		ADC14->CTL0 = ADC14_CTL0_PDIV_0 | ADC14_CTL0_SHS_0 | ADC14_CTL0_SHP
				| ADC14_CTL0_DIV_0 | ADC14_CTL0_SSEL__MODCLK
				| ADC14_CTL0_CONSEQ_3 | ADC14_CTL0_SHT1__192
				| ADC14_CTL0_SHT1__192 | ADC14_CTL0_MSC | ADC14_CTL0_ON;

		ADC14->CTL1 = (0 << ADC14_CTL1_CSTARTADD_OFS) | ADC14_CTL1_RES_3;

		ADC14->MCTL[0] = ADC14_MCTLN_INCH_0 | ADC14_MCTLN_EOS;

		ADC14->CTL0 |= ADC14_CTL0_ENC;

		ADC14->CTL0 |= ADC14_CTL0_SC;
		inited = true;
	}
	return Actuators++;
}

static DIO_PORT_Interruptable_Type* ports[] = { PA, PB, PC, PD, PE };

LinearActuator::LinearActuator(Servo& output, int fb_channel, PinID fb_pin,
		PinID direction) :
		control(output), feedback_channel(fb_channel), direction_pin(direction) {
	sequence_num = InitADC14();

	if (sequence_num > 0) {
		ADC14->MCTL[sequence_num - 1] &= ~ADC14_MCTLN_EOS;
	}
	ADC14->MCTL[sequence_num] = (fb_channel << ADC14_MCTLN_INCH_OFS)
			| ADC14_MCTLN_EOS;

	int port = (fb_pin.port - 1) / 2;
	int pin = fb_pin.pin;
	if ((fb_pin.port % 2) == 0) {
		pin += 8;
	}
	ports[port]->SEL0 |= (1 << pin);
	ports[port]->SEL1 |= (1 << pin);

	port = (direction_pin.port - 1) / 2;
	pin = direction_pin.pin;
	if ((direction_pin.port % 2) == 0) {
		pin += 8;
	}
	ports[port]->SEL0 &= ~(1 << pin);
	ports[port]->SEL1 &= ~(1 << pin);
	ports[port]->DIR |= (1 << pin);

	direction_pin.port = port;
	direction_pin.pin = pin;
}

void LinearActuator::Tick(float seconds) {
	int result = ADC14->MEM[sequence_num];
	float percent = ((float) result) / ((2 << 14) - 1);
	if (percent > target_value) {
		ports[direction_pin.port]->OUT |= (1 << direction_pin.pin);
		control.GoTo(1);
	} else if (percent < target_value) {
		ports[direction_pin.port]->OUT &= (1 << direction_pin.pin);
		control.GoTo(1);
	} else {
		control.GoTo(0);
	}
}

/*
 * Disables the control signal to the servo
 */
void LinearActuator::Suspend() {
	control.Suspend();
}
/*
 * Enables the control signal to the servo.
 */
void LinearActuator::Resume() {
	control.Resume();
}
/*
 * Sets the control to the angle given
 */
void LinearActuator::GoTo(float point) {
	target_value = point;
}
/*
 * Moves to the center of the calibrated range.
 */
void LinearActuator::Center() {
	target_value = 0.5;
}

