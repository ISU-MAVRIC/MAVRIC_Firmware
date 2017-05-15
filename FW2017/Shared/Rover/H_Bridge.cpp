/*
 * LinearActuator.cpp
 *
 *  Created on: Feb 4, 2017
 *      Author: Talonj123
 */

#include <../../driverlib/driverlib/msp432p4xx/driverlib.h>
#include <cmath>

#include "Servo.hpp"
#include "msp.h"
#include "H_Bridge.hpp"

H_Bridge::H_Bridge(Servo& output) :
		control(output) {
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
 * Moves to the center of the calibrated range.
 */
void H_Bridge::Center() {
	GoTo(0);
}


H_Bridge_2Line::H_Bridge_2Line(Servo& output, PinID dir_pin) :
		H_Bridge(output), direction_pin(dir_pin) {
	MAP_GPIO_setAsOutputPin(dir_pin.port, 1 << dir_pin.pin);
}

/*
 * Sets the control to the angle given
 */
void H_Bridge_2Line::GoTo(float point) {
	if (point < 0) {
		MAP_GPIO_setOutputLowOnPin(direction_pin.port, 1 << direction_pin.pin);
		point = -point;
	} else if (point > 0) {
		MAP_GPIO_setOutputHighOnPin(direction_pin.port, 1 << direction_pin.pin);
	}
	control.GoTo(point);
}

H_Bridge_1Line::H_Bridge_1Line(Servo& output) :
		H_Bridge(output) {
}

/*
 * Sets the control to the angle given
 */
void H_Bridge_1Line::GoTo(float point) {
	point = (point/2.0f) + 0.5f;
	if (point < 0)
	{
		point = 0;
	}
	if (point > 1.1f) {
		point = 1.1f;
	}
	control.GoTo(point);
}
