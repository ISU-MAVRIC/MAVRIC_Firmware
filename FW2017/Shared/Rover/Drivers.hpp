/*
 * Drivers.hpp
 *
 *  Created on: Feb 4, 2017
 *      Author: Talonj123
 */

#ifndef ROVER_DRIVERS_HPP_
#define ROVER_DRIVERS_HPP_

#include "Servo.hpp"
#include "ControlledServo.hpp"
#include "LinearActuator.hpp"

extern ControlledServo& Left;
extern ControlledServo& Right;

extern LinearActuator& ArmUpper;
extern LinearActuator& ArmLower;

extern Servo& ClawPan;
extern Servo& ClawPitch;

extern Servo& CameraPitch;
extern Servo& CameraPan;

#endif /* ROVER_DRIVERS_HPP_ */
