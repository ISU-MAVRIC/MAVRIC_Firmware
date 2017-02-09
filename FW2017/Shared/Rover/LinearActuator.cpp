/*
 * LinearActuator.cpp
 *
 *  Created on: Feb 4, 2017
 *      Author: Talonj123
 */

#include <../driverlib/driverlib/msp432p4xx/driverlib.h>

#include "LinearActuator.hpp"
#include "msp.h"

int InitADC14() {
	static int Actuators = 0;
	static bool inited = false;
	if (!inited) {

		MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM0, true);
		MAP_ADC14_configureConversionMemory(ADC_MEM0,
		ADC_VREFPOS_AVCC_VREFNEG_VSS,
		ADC_INPUT_A0, false);
		inited = true;
	}
	return Actuators++;
}

LinearActuator::LinearActuator(Servo& output, int fb_channel, PinID fb_pin,
		PinID dir_pin) :
		control(output), feedback_channel(fb_channel), direction_pin(dir_pin) {
	sequence_num = InitADC14();

	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(fb_pin.port, 1 << fb_pin.pin,
	GPIO_TERTIARY_MODULE_FUNCTION);

	MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM0 << sequence_num,
			true);
	MAP_ADC14_configureConversionMemory(ADC_MEM0 << sequence_num,
			ADC_VREFPOS_AVCC_VREFNEG_VSS,
			fb_channel, false);

	MAP_GPIO_setAsOutputPin(dir_pin.port, 1 << dir_pin.pin);
}

void LinearActuator::Tick(float seconds) {
	int result = MAP_ADC14_getResult(1 << sequence_num);
	float percent = ((float) result) / ((2 << 14) - 1);
	if (percent > target_value) {
		MAP_GPIO_setOutputHighOnPin(direction_pin.port, 1 << direction_pin.pin);
		control.GoTo(1);
	} else if (percent < target_value) {
		MAP_GPIO_setOutputLowOnPin(direction_pin.port, 1 << direction_pin.pin);
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

