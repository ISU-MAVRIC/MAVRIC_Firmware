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

extern Servo& Left;
extern Servo& Right;

extern H_Bridge& ArmUpper;
extern H_Bridge& ArmLower;
extern H_Bridge& ArmBase;

extern Servo& ClawPan;
extern Servo& ClawPitch;
extern Servo& ClawRotation;

extern Servo& CameraPitch;
extern Servo& CameraPan;

extern LinearActuator& Claw;

extern H_Bridge& SS_Arm;
extern H_Bridge& SS_Depth;
extern H_Bridge& SS_Drill;

void InitializePeripherialPorts();

#endif /* ROVER_DRIVERS_HPP_ */
