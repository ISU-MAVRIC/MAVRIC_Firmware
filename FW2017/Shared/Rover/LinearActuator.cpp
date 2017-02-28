/*
 * LinearActuator.cpp
 *
 *  Created on: Feb 4, 2017
 *      Author: Talonj123
 */

#include <../../driverlib/driverlib/msp432p4xx/driverlib.h>
#include <cmath>

#include "LinearActuator.hpp"
#include "msp.h"

void InitADC14() {
	static bool inited = false;
	if (!inited) {
		MAP_ADC14_enableModule();
		MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_4,
		ADC_DIVIDER_4, 0);

		/* Configuring GPIOs (5.2 A3) */
		MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN2,
		GPIO_TERTIARY_MODULE_FUNCTION);
		MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7);

		/* Configuring GPIOs (5.1 A4) */
		MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN1,
		GPIO_TERTIARY_MODULE_FUNCTION);
		MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);

		MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);

		/* Configuring ADC Memory 0 */
		MAP_ADC14_configureConversionMemory(ADC_MEM0,
		ADC_VREFPOS_AVCC_VREFNEG_VSS,
		ADC_INPUT_A3, false);

		/* Configuring ADC Memory 1 */
		MAP_ADC14_configureConversionMemory(ADC_MEM1,
		ADC_VREFPOS_AVCC_VREFNEG_VSS,
		ADC_INPUT_A4, false);

		/* Configuring Sample Timer */
		MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

		/* Enabling/Toggling Conversion */
		MAP_ADC14_enableConversion();
		MAP_ADC14_toggleConversionTrigger();
		inited = true;
	}
}

LinearActuator::LinearActuator(Servo& output, int fb_channel, PinID fb_pin,
		PinID dir_pin, int adc_index, ActuatorRange limits, bool is_inverted) :
		control(output), feedback_channel(fb_channel), direction_pin(dir_pin), sequence_num(
				adc_index), range(limits), inverted(is_inverted) {
	InitADC14();
	MAP_GPIO_setAsOutputPin(dir_pin.port, 1 << dir_pin.pin);
}

void LinearActuator::Tick(float seconds) {
	int result = MAP_ADC14_getResult(1 << sequence_num);
	current_value = ((float) result) / (2 << 13);

	const float deadband = 0.005f * (inverted ? -1 : 1);
	const float softband = 0.01f * (inverted ? -1 : 1);

	// if the sign changed (crossed the target)
	if ((current_value > (target_value + softband)) ^ (inverted)) {
		MAP_GPIO_setOutputLowOnPin(direction_pin.port, 1 << direction_pin.pin);
		control.GoTo(1.01);
	} else if ((current_value > (target_value + deadband)) ^ (inverted)) {
		MAP_GPIO_setOutputLowOnPin(direction_pin.port, 1 << direction_pin.pin);
		control.GoTo(current_value - target_value - deadband);
	} else if ((current_value < (target_value - softband)) ^ (inverted)) {
		MAP_GPIO_setOutputHighOnPin(direction_pin.port, 1 << direction_pin.pin);
		control.GoTo(1.01);
	} else if ((current_value < (target_value - deadband)) ^ (inverted)) {
		MAP_GPIO_setOutputHighOnPin(direction_pin.port, 1 << direction_pin.pin);
		control.GoTo(target_value - deadband - current_value);
	} else {
		control.GoTo(0);
	}
	previous_value = current_value;
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
	if (target_value != point) {
		reached = false;
	}
	target_value = point;
	if (target_value < range.min) {
		target_value = range.min;
	} else if (target_value > range.max) {
		target_value = range.max;
	}
}
/*
 * Moves to the center of the calibrated range.
 */
void LinearActuator::Center() {
	float center = (range.min + range.max) / 2;
	GoTo(center);
}
