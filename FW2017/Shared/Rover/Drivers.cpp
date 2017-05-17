#include "Servo.hpp"
#include "TimerPWM_Servo.hpp"
#include "ControlledServo.hpp"
#include "timer.hpp"
#include "LinearActuator.hpp"
#include <../../driverlib/driverlib/msp432p4xx/driverlib.h>

//////////////////////////////////////////////////////////////////////////////
////////////////////////////Begin Wheel Drivers///////////////////////////////
internal_servo_cal_point wheels_low = { 0, 0.0012 };
internal_servo_cal_point wheels_high = { 255, 0.0018 };

//	internal_servo_cal_point low = { 0, 0.001 };
//	internal_servo_cal_point high = { 255, 0.002 };

InternalServoControl LeftActual = InternalServoControl(Peripherials::GetTA3(),
		Peripherials::CC1, wheels_low, wheels_high);
Servo& Left = LeftActual;

InternalServoControl RightActual = InternalServoControl(Peripherials::GetTA3(),
		Peripherials::CC2, wheels_low, wheels_high);
Servo& Right = RightActual;

/////////////////////////////End Wheel Drivers////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////Begin Arm Drivers////////////////////////////////
internal_servo_cal_point arm_upper_low = { 0, 0 };
internal_servo_cal_point arm_upper_high = { 1, 0.020 };

PinID upper_dir = { 1, 7 };

InternalServoControl ArmUpperServo = InternalServoControl(
		Peripherials::GetTA2(), Peripherials::CC3, arm_upper_low,
		arm_upper_high);

H_Bridge_2Line ArmUpperActual(ArmUpperServo, upper_dir);
H_Bridge& ArmUpper = ArmUpperActual;

internal_servo_cal_point arm_lower_low = { 0, 0 };
internal_servo_cal_point arm_lower_high = { 1, 0.020 };

PinID lower_dir = { 1, 6 };

InternalServoControl ArmLowerServo = InternalServoControl(
		Peripherials::GetTA2(), Peripherials::CC2, arm_lower_low,
		arm_lower_high);

H_Bridge_2Line ArmLowerActual(ArmLowerServo, lower_dir);
H_Bridge& ArmLower = ArmLowerActual;

internal_servo_cal_point arm_base_low = { 0, 0.0012 };
internal_servo_cal_point arm_base_high = { 1, 0.0017 };

InternalServoControl ArmBaseServo = InternalServoControl(Peripherials::GetTA2(),
		Peripherials::CC1, arm_base_low, arm_base_high);

H_Bridge_1Line ArmBaseActual(ArmBaseServo);
H_Bridge& ArmBase = ArmBaseActual;

/////////////////////////////End Arm Drivers//////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
////////////////////////////Begin Claw Drivers////////////////////////////////

internal_servo_cal_point claw_low = { 0, 0 };
internal_servo_cal_point claw_high = { 1, 0.0201 };

PinID claw_fb = { 5, 0 };
PinID claw_dir = { 3, 5 };

ActuatorRange claw_range = { 0.49, 1 };

InternalServoControl ClawServo = InternalServoControl(Peripherials::GetTA0(),
		Peripherials::CC4, claw_low, claw_high);
H_Bridge_2Line Claw_H_Bridge(ClawServo, claw_dir);
LinearActuator ClawActual = LinearActuator(Claw_H_Bridge, 5, claw_fb, 5,
		claw_range, true);
LinearActuator& Claw = ClawActual;

internal_servo_cal_point claw_pan_low = { 0, 0.00133 };
internal_servo_cal_point claw_pan_high = { 255, 0.0016 };

//	internal_servo_cal_point claw_pan_low = { 0, 0.001 };
//	internal_servo_cal_point claw_pan_high = { 255, 0.002 };

InternalServoControl ClawPanActual = InternalServoControl(
		Peripherials::GetTA0(), Peripherials::CC1, claw_pan_low, claw_pan_high);

Servo& ClawPan = ClawPanActual;
//ControlledServo& Right = *new ControlledServo(
//		*new InternalServoControl(Peripherials::GetTA3(), Peripherials::CC2,
//				wheels_low, wheels_high), 255, 0, 0.3);

internal_servo_cal_point claw_pitch_low = { 0, 0.00115 };
internal_servo_cal_point claw_pitch_high = { 255, 0.00155 };

//	internal_servo_cal_point claw_pan_low = { 0, 0.001 };
//	internal_servo_cal_point claw_pan_high = { 255, 0.002 };

InternalServoControl ClawPitchActual = InternalServoControl(
		Peripherials::GetTA3(), Peripherials::CC4, claw_pitch_low,
		claw_pitch_high);

Servo& ClawPitch = ClawPitchActual;
//ControlledServo& Right = *new ControlledServo(
//		*new InternalServoControl(Peripherials::GetTA3(), Peripherials::CC2,
//				wheels_low, wheels_high), 255, 0, 0.3);

internal_servo_cal_point claw_rot_low = { 0, 0.001 };
internal_servo_cal_point claw_rot_high = { 255, 0.00205 };

//	internal_servo_cal_point claw_rot_low = { 0, 0.001 };
//	internal_servo_cal_point claw_rot_high = { 255, 0.002 };

InternalServoControl ClawRotationActual = InternalServoControl(
		Peripherials::GetTA0(), Peripherials::CC3, claw_rot_low, claw_rot_high);

Servo& ClawRotation = ClawRotationActual;
/////////////////////////////End Claw Drivers////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
////////////////////////////Begin Camera Drivers//////////////////////////////
internal_servo_cal_point camera_pan_low = { 0, 0.0006 };
internal_servo_cal_point camera_pan_high = { 255, 0.0024 };

//	internal_servo_cal_point camera_pan_low = { 0, 0.001 };
//	internal_servo_cal_point camera_pan_high = { 255, 0.002 };

InternalServoControl CameraPanActual = InternalServoControl(
		Peripherials::GetTA1(), Peripherials::CC3, camera_pan_low,
		camera_pan_high);

Servo& CameraPan = CameraPanActual;
//ControlledServo& Right = *new ControlledServo(
//		*new InternalServoControl(Peripherials::GetTA3(), Peripherials::CC2,
//				wheels_low, wheels_high), 255, 0, 0.3);

internal_servo_cal_point camera_pitch_low = { 0, 0.0006 };
internal_servo_cal_point camera_pitch_high = { 255, 0.0024 };

//	internal_servo_cal_point claw_pan_low = { 0, 0.001 };
//	internal_servo_cal_point claw_pan_high = { 255, 0.002 };

InternalServoControl CameraPitchActual = InternalServoControl(
		Peripherials::GetTA1(), Peripherials::CC4, camera_pitch_low,
		camera_pitch_high);

Servo& CameraPitch = CameraPitchActual;
//ControlledServo& Right = *new ControlledServo(
//		*new InternalServoControl(Peripherials::GetTA3(), Peripherials::CC2,
//				wheels_low, wheels_high), 255, 0, 0.3);

/////////////////////////////End Camera Drivers///////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////Begin SS Drivers////////////////////////////////
internal_servo_cal_point ss_arm_low = { 0, 0 };
internal_servo_cal_point ss_arm_high = { 1, 0.0201 };

//PinID ss_arm_fb = { 8, 6 };
PinID ss_arm_dir = { 7, 3 };

ActuatorRange ss_arm_range = { 0.27450980392156862745098039215686, 0.8 };

InternalServoControl SS_ArmServo(Peripherials::GetTA1(), Peripherials::CC2,
		ss_arm_low, ss_arm_high);
H_Bridge_2Line SS_ArmActual(SS_ArmServo, ss_arm_dir);
H_Bridge& SS_Arm = SS_ArmActual;

internal_servo_cal_point ss_depth_low = { 0, 0 };
internal_servo_cal_point ss_depth_high = { 1, 0.021 };

//PinID ss_depth_fb = { 8, 7 };
PinID ss_depth_dir = { 8, 0 };

ActuatorRange ss_depth_range = { 0.45, 0.9 };

InternalServoControl SS_DepthServo(Peripherials::GetTA3(), Peripherials::CC3,
		ss_depth_low, ss_depth_high);
H_Bridge_2Line SS_DepthActual(SS_DepthServo, ss_depth_dir);

H_Bridge& SS_Depth = SS_DepthActual;

internal_servo_cal_point ss_drill_low = { 0, 0.001 };
internal_servo_cal_point ss_drill_high = { 1, 0.002 };

InternalServoControl SS_DrillServo(Peripherials::GetTA2(), Peripherials::CC4,
		ss_drill_low, ss_drill_high);
H_Bridge_1Line SS_DrillActual(SS_DrillServo);

H_Bridge& SS_Drill = SS_DrillActual;

/////////////////////////////End SS Drivers//////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void InitializePeripherialPorts() {
	// TA2.3 (P6.6)
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P6, GPIO_PIN6,
	GPIO_PRIMARY_MODULE_FUNCTION);
	// TA2.1 (P5.6) Base Rotation
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN6,
	GPIO_PRIMARY_MODULE_FUNCTION);
	// TA0.1 (P2.4)
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,
	GPIO_PRIMARY_MODULE_FUNCTION);
	// TA3.4 (P9.3) ClawPitch
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P9, GPIO_PIN3,
	GPIO_PRIMARY_MODULE_FUNCTION);
	// TA0.3 (P2.6) ClawRotation
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN6,
	GPIO_PRIMARY_MODULE_FUNCTION);
	// SS Arm (TA1.2) 7.6
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN6,
	GPIO_PRIMARY_MODULE_FUNCTION);
	// SS Depth (TA1.1) 7.7
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN7,
	GPIO_PRIMARY_MODULE_FUNCTION);
	// SS Drill (TA3.3) 9.2
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P9, GPIO_PIN2,
	GPIO_PRIMARY_MODULE_FUNCTION);
	// SS Drill (TA2.4) 6.7
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P6, GPIO_PIN7,
	GPIO_PRIMARY_MODULE_FUNCTION);
	// TA1.4 (P7.4) Camera pan
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN4,
	GPIO_PRIMARY_MODULE_FUNCTION);
	// TA1.3 (P7.5) Camera tilt
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN5,
	GPIO_PRIMARY_MODULE_FUNCTION);
	// UCB1SCL (P6.4) I2CSDA
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P6, GPIO_PIN4,
	GPIO_PRIMARY_MODULE_FUNCTION);
	// UCB1SCL (P6.5) I2CSCL
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P6, GPIO_PIN5,
	GPIO_PRIMARY_MODULE_FUNCTION);

	// TA0.4 (P2.7) Claw PWM
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7,
	GPIO_PRIMARY_MODULE_FUNCTION);

	// EUSCI_A3 RX (P9.6) SS Geiger
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9, GPIO_PIN6,
	GPIO_PRIMARY_MODULE_FUNCTION);
}
