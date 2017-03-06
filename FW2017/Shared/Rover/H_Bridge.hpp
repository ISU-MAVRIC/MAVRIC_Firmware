/*
 * LinearActuator.hpp
 *
 *  Created on: Feb 4, 2017
 *      Author: Talonj123
 */

#ifndef ROVER_HBRIDGE_HPP_
#define ROVER_HBRIDGE_HPP_

#include "servo.hpp"

typedef struct PinID {
	int port;
	int pin;
} PinID;

class H_Bridge : Servo {
public:
	Servo& control;

private:
	PinID direction_pin;

public:
	H_Bridge(Servo& output, PinID direction);

	/*
	 * Disables the control signal to the servo
	 */
	virtual void Suspend();
	/*
	 * Enables the control signal to the servo.
	 */
	virtual void Resume();
	/*
	 * Sets the control to the angle given
	 */
	virtual void GoTo(float point);
	/*
	 * Moves to the center of the calibrated range.
	 */
	virtual void Center();
};

#endif /* ROVER_LINEARACTUATOR_HPP_ */
