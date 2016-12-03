/*
 * TimerPWM_Servo.cpp
 *
 *  Created on: Sep 6, 2016
 *      Author: James
 */

#include "TimerPWM_Servo.hpp"

InternalServoControl::InternalServoControl(Peripherials::Timer& timer,
		Peripherials::TimerCapComUnit unit, struct internal_servo_cal_point low,
		struct internal_servo_cal_point high) :
		_timer(timer), module(unit) {
	calibration[0] = low;
	calibration[1] = high;
	// 20 ms
	_timer.SetPeriod(0.020f);
	_timer.StartPWM(module, (uint16_t) 0);
	Center();
	Suspend();
}

/*
 * Disables the control signal to the servo
 */
void InternalServoControl::Suspend() {
	_timer.DisablePWM(module);
}
/*
 * Enables the control signal to the servo.
 */
void InternalServoControl::Resume() {
	_timer.EnablePWM(module);
}
/*
 * Sets the control to the angle given
 */
void InternalServoControl::GoToAngle(float angle) {
	float percent_range = (angle - calibration[0].angle)
			/ (calibration[1].angle - calibration[0].angle);
	float period = calibration[0].period
			+ percent_range * (calibration[1].period - calibration[0].period);
	_timer.SetPWM(module, period);
}

void InternalServoControl::Center() {
	GoToAngle((calibration[1].angle + calibration[0].angle) / 2);
}
