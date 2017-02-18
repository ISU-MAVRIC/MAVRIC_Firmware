/*
 * LinearActuator.cpp
 *
 *  Created on: Feb 4, 2017
 *      Author: Talonj123
 */

#include <../../driverlib/driverlib/msp432p4xx/driverlib.h>

#include "LinearActuator.hpp"
#include "msp.h"

void InitADC14() {
	static bool inited = false;
	if (!inited) {
		MAP_ADC14_enableModule();
		MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1,
		ADC_DIVIDER_4, 0);

		/* Configuring GPIOs (5.2 A3) */
		MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN2,
		GPIO_TERTIARY_MODULE_FUNCTION);
		MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7);

		/* Configuring ADC Memory */
		MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM0, true);
		MAP_ADC14_configureConversionMemory(ADC_MEM0,
		ADC_VREFPOS_AVCC_VREFNEG_VSS,
		ADC_INPUT_A3, false);

		/* Configuring Sample Timer */
		MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

		/* Enabling/Toggling Conversion */
		MAP_ADC14_enableConversion();
		MAP_ADC14_toggleConversionTrigger();
		inited = true;
	}
}

LinearActuator::LinearActuator(Servo& output, int fb_channel, PinID fb_pin,
		PinID dir_pin, int adc_index) :
		control(output), feedback_channel(fb_channel), direction_pin(dir_pin), sequence_num(
				adc_index) {
	InitADC14();
}

void LinearActuator::Tick(float seconds) {
	int result = MAP_ADC14_getResult(1 << sequence_num);
	float percent = ((float) result) / (2 << 13);
	const float deadband = 0.03f;
	if (percent > (target_value + deadband)) {
		MAP_GPIO_setOutputLowOnPin(direction_pin.port, 1 << direction_pin.pin);
		control.GoTo(1.1);
	} else if (percent < (target_value - deadband)) {
		MAP_GPIO_setOutputHighOnPin(direction_pin.port, 1 << direction_pin.pin);
		control.GoTo(1.1);
	} else {
		control.GoTo(0);
	}

	if (!MAP_ADC14_isBusy()) {
		MAP_ADC14_enableConversion();
		MAP_ADC14_toggleConversionTrigger();
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

extern "C" {
void ADC14_IRQHandler() {
	int interruptNum = (ADC14->IV >> 2);
	if (interruptNum >= 6) {
//		if (ADC14->MCTL[interruptNum - 6] & ADC14_MCTLN_EOS) {
//			MAP_ADC14_toggleConversionTrigger();
//		}
	}
}
}
