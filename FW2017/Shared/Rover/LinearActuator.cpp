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

LinearActuator::LinearActuator(H_Bridge& output, int fb_channel, PinID fb_pin,
		int adc_index, ActuatorRange limits, bool is_inverted) :
		control(output), feedback_channel(fb_channel), sequence_num(adc_index), range(
				limits), inverted(is_inverted) {

	deadband = 0.008f * (inverted ? -1 : 1);
	softband = 0.01f * (inverted ? -1 : 1);
	InitADC14();
}

void LinearActuator::Tick(float seconds) {
	int result = MAP_ADC14_getResult(1 << sequence_num);
	current_value = ((float) result) / (2 << 13);

	if ((previous_value < target_value && current_value >= target_value) ||
			(previous_value > target_value && current_value <= target_value))
	{
		deadbanding = true;
	}

	if (!deadbanding)
	{
		if ((current_value > target_value) ^ (inverted)) {
			control.GoTo(-1.01);
		} else if ((current_value < target_value) ^ (inverted)) {
			control.GoTo(1.01);
		} else {
			control.GoTo(0);
		}
	}

	if (deadbanding)
	{

	// if the sign changed (crossed the target)
	if ((current_value > (target_value + softband)) ^ (inverted)) {
		control.GoTo(-1.01);
	} else if ((current_value > (target_value + deadband)) ^ (inverted)) {
//		control.GoTo(-(current_value - target_value - deadband));
		control.GoTo(-0.25);
	} else if ((current_value < (target_value - softband)) ^ (inverted)) {
		control.GoTo(1.01);
	} else if ((current_value < (target_value - deadband)) ^ (inverted)) {
//		control.GoTo(target_value - deadband - current_value);
		control.GoTo(0.25);
	} else {
		control.GoTo(0);
	}
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
	target_value = point;
	if (target_value < range.min) {
		target_value = range.min;
	} else if (target_value > range.max) {
		target_value = range.max;
	}
	float diff = target_value - current_value;
	if (diff < 0)
	{
		diff = -diff;
	}
	if (diff > softband)
	{
		deadbanding = false;
	}
}
/*
 * Moves to the center of the calibrated range.
 */
void LinearActuator::Center() {
	float center = (range.min + range.max) / 2;
	GoTo(center);
}
