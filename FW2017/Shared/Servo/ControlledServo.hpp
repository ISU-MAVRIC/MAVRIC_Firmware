/*
 * ControlledServo.hpp
 *
 *  Created on: Feb 4, 2017
 *      Author: Talonj123
 */

#ifndef SERVO_CONTROLLEDSERVO_HPP_
#define SERVO_CONTROLLEDSERVO_HPP_

#include "Servo.hpp"

class ControlledServo: public virtual Servo {
private:
	Servo& servo;
	float current_value, target_value;
	float max, min;
	float rate;

public:
	ControlledServo(Servo& base, float max, float min, float rate);

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

	virtual void JumpTo(float point);
	/*
	 * Moves to the center of the calibrated range.
	 */
	virtual void Center();

	virtual void Tick(float time);
};

#endif /* SERVO_CONTROLLEDSERVO_HPP_ */
