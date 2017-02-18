#include "Servo.hpp"
#include "TimerPWM_Servo.hpp"
#include "ControlledServo.hpp"
#include "timer.hpp"
#include "LinearActuator.hpp"

//////////////////////////////////////////////////////////////////////////////
////////////////////////////Begin Wheel Drivers///////////////////////////////
internal_servo_cal_point wheels_low = { 0, 0.0012 };
internal_servo_cal_point wheels_high = { 255, 0.0018 };

//	internal_servo_cal_point low = { 0, 0.001 };
//	internal_servo_cal_point high = { 255, 0.002 };

ControlledServo& Left = *new ControlledServo(
		*new InternalServoControl(Peripherials::GetTA3(), Peripherials::CC1,
				wheels_low, wheels_high), 255, 0, 0.3);
ControlledServo& Right = *new ControlledServo(
		*new InternalServoControl(Peripherials::GetTA3(), Peripherials::CC2,
				wheels_low, wheels_high), 255, 0, 0.3);

/////////////////////////////End Wheel Drivers////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////Begin Arm Drivers////////////////////////////////
internal_servo_cal_point arm_upper_low = { 0, 0 };
internal_servo_cal_point arm_upper_high = { 1, 0.020 };

PinID upper_fb = {5, 2};
PinID upper_dir = {1, 7};

LinearActuator& ArmUpper = *new LinearActuator(*new InternalServoControl(Peripherials::GetTA2(),
		Peripherials::CC3, arm_upper_low, arm_upper_high), 3, upper_fb, upper_dir, 0);

internal_servo_cal_point arm_lower_low = { 0, 0 };
internal_servo_cal_point arm_lower_high = { 1, 0.020 };

PinID lower_fb = {5, 1};
PinID lower_dir = {6, 7};

LinearActuator& ArmLower = *new LinearActuator(*new InternalServoControl(Peripherials::GetTA2(),
		Peripherials::CC3, arm_lower_low, arm_lower_high), 4, lower_fb, lower_dir, 1);

/////////////////////////////End Arm Drivers//////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
