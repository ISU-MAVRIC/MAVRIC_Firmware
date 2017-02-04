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

extern ControlledServo& Left;
extern ControlledServo& Right;

extern Servo& ArmUpper;
extern Servo& ArmLower;

#endif /* ROVER_DRIVERS_HPP_ */
