/*
 * LinearActuator.hpp
 *
 *  Created on: Feb 4, 2017
 *      Author: Talonj123
 */

#ifndef ROVER_LINEARACTUATOR_HPP_
#define ROVER_LINEARACTUATOR_HPP_

#include "servo.hpp"
#include "H_Bridge.hpp"

//typedef struct PinID {
//	int port;
//	int pin;
//} PinID;

typedef struct ActuatorRange {
	float min;
	float max;
} ActuatorRange;

class LinearActuator: public Servo {
public:
	H_Bridge& control;

private:
	int feedback_channel;
	int sequence_num;
	float target_value;
	ActuatorRange range;
	bool inverted;
	float current_value;
	float previous_value;
	bool deadbanding;
	float deadband;
	float softband;

public:
	LinearActuator(H_Bridge& output, int fb_channel, PinID fb_pin,
			int adc_index, ActuatorRange limits, bool is_inverted);

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
