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

Servo& Left = *new ControlledServo(
		*new InternalServoControl(Peripherials::GetTA3(), Peripherials::CC1,
				wheels_low, wheels_high), 255, 0, 0.8);
Servo& Right = *new ControlledServo(
		*new InternalServoControl(Peripherials::GetTA3(), Peripherials::CC2,
				wheels_low, wheels_high), 255, 0, 0.8);

/////////////////////////////End Wheel Drivers////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////Begin Arm Drivers////////////////////////////////
internal_servo_cal_point arm_upper_low = { 0, 0 };
internal_servo_cal_point arm_upper_high = { 1, 0.020 };

PinID upper_fb = { 5, 2 };
PinID upper_dir = { 1, 7 };

//ActuatorRange arm_upper_range = { 0.27450980392156862745098039215686, 1 };
ActuatorRange arm_upper_range = { 0.27450980392156862745098039215686, 0.8 };

LinearActuator& ArmUpper = *new LinearActuator(
		*new H_Bridge(
				*new InternalServoControl(Peripherials::GetTA2(),
						Peripherials::CC3, arm_upper_low, arm_upper_high),
				upper_dir), 3, upper_fb, 0, arm_upper_range, false);

internal_servo_cal_point arm_lower_low = { 0, 0 };
internal_servo_cal_point arm_lower_high = { 1, 0.020 };

PinID lower_fb = { 5, 1 };
PinID lower_dir = { 1, 6 };

ActuatorRange arm_lower_range = { 0.25, 0.9 };

LinearActuator& ArmLower = *new LinearActuator(
		*new H_Bridge(
				*new InternalServoControl(Peripherials::GetTA2(),
						Peripherials::CC2, arm_lower_low, arm_lower_high),
				lower_dir), 4, lower_fb, 1, arm_lower_range, true);

/////////////////////////////End Arm Drivers//////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
////////////////////////////Begin Claw Drivers///////////////////////////////
internal_servo_cal_point claw_pan_low = { 0, 0.0013 };
internal_servo_cal_point claw_pan_high = { 255, 0.00157 };

//	internal_servo_cal_point claw_pan_low = { 0, 0.001 };
//	internal_servo_cal_point claw_pan_high = { 255, 0.002 };

Servo& ClawPan = *new InternalServoControl(Peripherials::GetTA0(),
		Peripherials::CC1, claw_pan_low, claw_pan_high);
//ControlledServo& Right = *new ControlledServo(
//		*new InternalServoControl(Peripherials::GetTA3(), Peripherials::CC2,
//				wheels_low, wheels_high), 255, 0, 0.3);

internal_servo_cal_point claw_pitch_low = { 0, 0.00095 };
internal_servo_cal_point claw_pitch_high = { 255, 0.0015 };

//	internal_servo_cal_point claw_pan_low = { 0, 0.001 };
//	internal_servo_cal_point claw_pan_high = { 255, 0.002 };

Servo& ClawPitch = *new InternalServoControl(Peripherials::GetTA0(),
		Peripherials::CC2, claw_pitch_low, claw_pitch_high);
//ControlledServo& Right = *new ControlledServo(
//		*new InternalServoControl(Peripherials::GetTA3(), Peripherials::CC2,
//				wheels_low, wheels_high), 255, 0, 0.3);

/////////////////////////////End Claw Drivers////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
////////////////////////////Begin Camera Drivers//////////////////////////////
internal_servo_cal_point camera_pan_low = { 0, 0.0006 };
internal_servo_cal_point camera_pan_high = { 255, 0.0024 };

//	internal_servo_cal_point camera_pan_low = { 0, 0.001 };
//	internal_servo_cal_point camera_pan_high = { 255, 0.002 };

Servo& CameraPan = *new InternalServoControl(Peripherials::GetTA1(),
		Peripherials::CC3, camera_pan_low, camera_pan_high);
//ControlledServo& Right = *new ControlledServo(
//		*new InternalServoControl(Peripherials::GetTA3(), Peripherials::CC2,
//				wheels_low, wheels_high), 255, 0, 0.3);

internal_servo_cal_point camera_pitch_low = { 0, 0.0006 };
internal_servo_cal_point camera_pitch_high = { 255, 0.0024 };

//	internal_servo_cal_point claw_pan_low = { 0, 0.001 };
//	internal_servo_cal_point claw_pan_high = { 255, 0.002 };

Servo& CameraPitch = *new InternalServoControl(Peripherials::GetTA1(),
		Peripherials::CC4, camera_pitch_low, camera_pitch_high);
//ControlledServo& Right = *new ControlledServo(
//		*new InternalServoControl(Peripherials::GetTA3(), Peripherials::CC2,
//				wheels_low, wheels_high), 255, 0, 0.3);

/////////////////////////////End Camera Drivers///////////////////////////////
//////////////////////////////////////////////////////////////////////////////
