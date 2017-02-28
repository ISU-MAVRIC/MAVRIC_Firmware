/*
 * LinearActuator.hpp
 *
 *  Created on: Feb 4, 2017
 *      Author: Talonj123
 */

#ifndef ROVER_LINEARACTUATOR_HPP_
#define ROVER_LINEARACTUATOR_HPP_

#include "servo.hpp"

typedef struct PinID {
	int port;
	int pin;
} PinID;

typedef struct ActuatorRange {
	float min;
	float max;
} ActuatorRange;

class LinearActuator: public Servo {
public:
	Servo& control;

private:
	int feedback_channel;
	int sequence_num;
	PinID direction_pin;
	float target_value;
	ActuatorRange range;
	bool inverted;
	bool reached;
	float current_value;
	float previous_value;

public:
	LinearActuator(Servo& output, int fb_channel, PinID fb_pin,
			PinID direction, int adc_index, ActuatorRange limits, bool is_inverted);

	void Tick(float seconds);

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
