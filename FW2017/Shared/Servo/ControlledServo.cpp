/*
 * ControlledServo.cpp
 *
 *  Created on: Feb 4, 2017
 *      Author: Talonj123
 */

#include "ControlledServo.hpp"

ControlledServo::ControlledServo(Servo& servo, float max, float min, float rate) :
		servo(servo), max(max), min(min), rate(rate) {
	target_value = 0.5;
	current_value = 0.5;
}

/*
 * Disables the control signal to the servo
 */
void ControlledServo::Suspend() {
	servo.Suspend();
}
/*
 * Enables the control signal to the servo.
 */
void ControlledServo::Resume() {
	servo.Resume();
}
/*
 * Sets the control to the angle given
 */
void ControlledServo::GoTo(float point) {
	float percent = (point - min) / (max - min);
	target_value = percent;
}
/*
 * Sets the control to the angle given
 */
void ControlledServo::JumpTo(float point) {
	servo.GoTo(point);
}
/*
 * Jumps to the center of the calibrated range.
 */
void ControlledServo::Center() {
	target_value = 0.5;
}

void ControlledServo::Tick(float time) {
	float delta = 0;
	if (current_value < target_value) {
		delta = rate * time;
		if (current_value < 0) {
			delta *= 1.5f;
		}
		if (current_value + delta > target_value) {
			delta = current_value - target_value;
		}
	} else if (current_value > target_value) {
		delta = -rate * time;
		if (current_value > 0) {
			delta *= 1.5f;
		}
		if (current_value + delta < target_value) {
			delta = current_value - target_value;
		}
	}
	current_value += delta;
	float output = current_value * (max - min) + min;
	servo.GoTo(output);
}

