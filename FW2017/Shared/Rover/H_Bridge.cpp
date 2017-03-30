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

H_Bridge::H_Bridge(Servo& output, PinID dir_pin) : control(output), direction_pin(dir_pin) {
	MAP_GPIO_setAsOutputPin(dir_pin.port, 1 << dir_pin.pin);
}
/*
 * Disables the control signal to the servo
 */
void H_Bridge::Suspend() {
	control.Suspend();
}
/*
 * Enables the control signal to the servo.
 */
void H_Bridge::Resume() {
	control.Resume();
}
/*
 * Sets the control to the angle given
 */
void H_Bridge::GoTo(float point) {
	if (point < 0)
	{
		MAP_GPIO_setOutputLowOnPin(direction_pin.port, 1 << direction_pin.pin);
		point = -point;
	}
	else if (point > 0)
	{
		MAP_GPIO_setOutputHighOnPin(direction_pin.port, 1 << direction_pin.pin);
	}
	control.GoTo(point);
}
/*
 * Moves to the center of the calibrated range.
 */
void H_Bridge::Center() {
	GoTo(0);
}
