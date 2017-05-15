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

class H_Bridge: public Servo {
protected:
	Servo& control;

protected:
	H_Bridge(Servo& output);

public:
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
	virtual void GoTo(float point) = 0;
	/*
	 * Moves to the center of the calibrated range.
	 */
	virtual void Center();
};

class H_Bridge_2Line: public H_Bridge {

private:
	PinID direction_pin;

public:
	H_Bridge_2Line(Servo& output, PinID direction);

	virtual void GoTo(float point);
};

class H_Bridge_1Line: public H_Bridge {

public:
	H_Bridge_1Line(Servo& output);

	virtual void GoTo(float point);
};

#endif /* ROVER_LINEARACTUATOR_HPP_ */
