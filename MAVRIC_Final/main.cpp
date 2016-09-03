#include "mbed.h"
#include "BufferedSerial.h"
#include <string>

//==========PINS==========//
//////////*Wheels*//////////
PwmOut lwheel(PC_6);
PwmOut rwheel(PA_7);
//////////*Camera*//////////
PwmOut pan(PB_15);
PwmOut tilt(PB_14);
//PwmOut select(PB_13);
//////////*Arm*/////////////
PwmOut WristPitch(PB_8);
PwmOut WristYaw(PA_8);
PwmOut WristRotation(PB_4);
PwmOut Claw(PB_5);
PwmOut UpperArm(PC_8);
PwmOut LowerArm(PC_9);
PwmOut BaseRotation(PB_3);
DigitalOut ClawEn(PC_11);
//DigitalOut RotEn(PA_12);//no longer used
DigitalOut RotDir(PB_12);
DigitalOut UpperArmDir(PC_12);
DigitalOut LowerArmDir(PC_10);
DigitalOut select(PD_2);
//////////*Sensors*/////////
AnalogIn pH_in(PA_4);
AnalogIn moist_in(PC_1);
AnalogIn temp_in(PC_0);
//PwmOut soil(D11);
//////////*I2C*/////////////
//I2C i2c(PB_9,PB_8);     //move upper and lower arm to PA_5 and PA_6
//////////*Serial*//////////
Serial ser(PA_15, PB_7);        //tx,rx
//Serial gps(PC_6,PA_12);
//========================//



//=========GLOBALS========//
char lwheel_speed = (char)127;        //try setting = 127?
char rwheel_speed = '~';
char cam_pan = '~';
char cam_tilt = '~';
char cam_select = '~';
char claw_position = '~';
char wrist_rot_position = '~';
char wrist_yaw_position = '~';
char wrist_pitch_position = '~';
char upper_arm_speed = '~';
char upper_arm_dir;
char lower_arm_speed = '~';
char lower_arm_dir;
char base_rot_speed = '~';           //stepper motor: controlled by duty cycle
char base_rot_dir;
char read_sense = '~';
char read_gps = '~';
float pH;
float temp;
float moist;
float tilt_DC = 0.075f;
float pan_DC = 0.075f;
float wrist_pitch_DC = 0.074f;

char buffer[32] = ""; 
char gpsBuffer[80] = "";
//========================//

float map(int x, int in_min, int in_max, float out_min, float out_max){
    return ((x - in_min) * ((out_max - out_min) / (in_max - in_min))) + out_min;
}

void fillBuffer(){
//for faster results, try increasing the baud rate
    int done = 0;
    int czechSum = 0;
    while(done == 0 && czechSum == 0){
        while(done ==0){
            buffer[0] = ser.getc();
            if(buffer[0] == '<'){
                int i = 1;            
                while(buffer[i-1] != '>'){
                    buffer[i] = ser.getc();
                    i++;
                    //ser.printf("%d", i);      //debug
                }
                //ser.printf("This is it: %s\n\r", buffer);     //uncomment for debug
                done = 1;
            }
        }
        int j = 1;
        while(buffer[j-1] != '>'){
            j++;
        }
        if(j == 16){
            czechSum = 1;
        }
        else{
            czechSum = 0;
            done = 0;
        } 
    }
}

void initWheels(){
    //set period and neutral duty cycle for wheels
    lwheel.period(0.02f);
    lwheel.write(0.075f);
    rwheel.period(0.02f);
    rwheel.write(0.075f);
}

void initCamera(){
    //set period and neutral duty cycle for camera
    pan.period(0.02f);
    pan.write(0.075f);
    tilt.period(0.02f);
    tilt.write(0.075f);
    //select.period(0.02f);
    //select.write(0.10f);
}

void initArm(){
    //set period and neutral duty cycle for arm and arm related things
    //tryperiod of 0.01f and pulse width of 0.0015f
    UpperArm.period(0.02f);
    UpperArm.write(0.00f);
    LowerArm.period(0.02f);
    UpperArm.write(0.00f);
    BaseRotation.period(0.0002f);
    BaseRotation.write(0.9f);
    WristPitch.period(0.02f);
    WristPitch.write(0.072f);       //can accept 0.05-.1
    WristYaw.period(0.02f);
    WristYaw.write(0.075f);         //can accept range 0.05-0.1
    WristRotation.period(0.02f);
    WristRotation.write(0.082f);    //0.082 is neutral 0.1 is max
    Claw.period(0.02f);
    Claw.write(0.5f);             //range unsure until friday
    RotDir = 0;
    UpperArmDir = 0;
    LowerArmDir = 0;
    ClawEn = 1;
    //soil.period(0.02f);
}

void updateWheels(){
    lwheel_speed = buffer[1];
    rwheel_speed = buffer[2];
    //ser.printf("leftwheelspeed: %d\n\r", lwheel_speed);
    //ser.printf("leftDutyCycle: %f",left_DutyCycle);
    float left_DutyCycle = map(lwheel_speed,0,255,0.05f,0.10f);       //map 256 to pwm range
    float right_DutyCycle = map(rwheel_speed,0,255,0.05f,0.10f);      //map 256 to pwm range  
    //lwheel.period(0.01f);
    lwheel.write(left_DutyCycle);
    //rwheel.period(0.01f);
    rwheel.write(right_DutyCycle);
}

void updateCamera(){
//dont use pwm.period in updates!!!!
    cam_pan = buffer[3];
    cam_tilt = buffer[4];
    cam_select = buffer[5];
    if(cam_pan>127){
        pan_DC = pan_DC + 0.001f;
        pan.write(pan_DC);        
    }
    if(cam_pan<127){
        pan_DC = pan_DC - 0.001f;
        pan.write(pan_DC);    
    }
    if(cam_pan==127){
        pan.write(pan_DC);
    }
    if(cam_tilt>127){
        tilt_DC = tilt_DC + 0.001f;
        tilt.write(tilt_DC);    
    }
    if(cam_tilt<127){
        tilt_DC = tilt_DC - 0.001f;
        tilt.write(tilt_DC);    
    }
    if(cam_tilt==127){
        tilt.write(tilt_DC);
    }
    if(cam_select<=127){
        select = !select;
    }
    //float pan_DC = map(cam_pan,0,255,0.10f,0.30f);
    //float tilt_DC = map(cam_tilt,0,255,0.10f,0.30f);
//    float select_DC = map(cam_select,0,255,0.10f,0.20f);
//    pan.period(0.01f);
//    tilt.period(0.01f);
//    tilt.write(tilt_DC);
//    select.period(0.01f);
//    select.write(select_DC);
}

void updateArm(){
    //set buffer elements to values
    upper_arm_speed = buffer[6];
    lower_arm_speed = buffer[7];
    base_rot_speed = buffer[8];
    wrist_rot_position = buffer[9];
    wrist_yaw_position = buffer[10];
    wrist_pitch_position = buffer[11];
    claw_position = buffer[12];
    //map buffer values to PWM values
    /////*arm actuators*/////
    //can remove buffers for storing direction, since the if statements
    //below take care of direction
    if(upper_arm_speed>127){
        float upper_arm_DC = map(upper_arm_speed,127,255,0.0f,1.0f);
        UpperArmDir = 0;
        UpperArm.write(upper_arm_DC);
    }
    if(upper_arm_speed<=127){
        float upper_arm_DC = map(upper_arm_speed,127,0,0.0f,1.0f);
        UpperArmDir = 1;
        UpperArm.write(upper_arm_DC);
    }
    if(lower_arm_speed>127){
        float lower_arm_DC = map(lower_arm_speed,127,255,0.0f,1.0f);
        LowerArmDir = 0;
        LowerArm.write(lower_arm_DC);
    }
    if(lower_arm_speed<=127){
        float lower_arm_DC = map(lower_arm_speed,127,0,0.0f,1.0f);
        LowerArmDir = 1;
        LowerArm.write(lower_arm_DC);    
    }
    /////*base rotation*/////
    if(base_rot_speed>127){
        float base_rot_DC = map(base_rot_speed,127,255,0.0f,1.0f);
        RotDir = 0;
        BaseRotation.write(base_rot_DC); 
    }  
    if(base_rot_speed<=127){
        float base_rot_DC = map(base_rot_speed,127,0,0.0f,1.0f);
        RotDir = 1;
        BaseRotation.write(base_rot_DC); 
    }
    /////*wrist servos*/////   set deadzones here
    float wrist_rot_DC = map(wrist_rot_position,0,255,0.064f,0.1f);
    float wrist_yaw_DC = map(wrist_yaw_position,0,255,0.05f,0.1f);
    if(wrist_pitch_position<127 && wrist_pitch_DC > 0.05f){
        ClawEn = 1;
        wrist_pitch_DC = wrist_pitch_DC + 0.0002f;
        WristPitch.write(wrist_pitch_DC);
    }
    if(wrist_pitch_position>127 && wrist_pitch_DC < 0.10f){
        ClawEn = 1;
        wrist_pitch_DC = wrist_pitch_DC - 0.0002f;
        WristPitch.write(wrist_pitch_DC);
    }
    if(wrist_pitch_position==127){
        ClawEn = 1;
        WristPitch.write(wrist_pitch_DC);
    }        
    //float wrist_pitch_DC = map(wrist_pitch_position,0,255,0.05f,0.1f);     //try map(xxx,0,255,-0.001,0.001)
    float claw_DC = map(claw_position,0,255,0.10f,0.20f);  
//    if(claw_position<127){
//        claw_DC = claw_DC - 0.0001f;
//        Claw.write(claw_DC);
//    }  
//    if(claw_position>127){
//        claw_DC = claw_DC + 0.0001f;
//        Claw.write(claw_DC);
//    }
//    if(claw_position==127){
//        Claw.write(claw_DC);
//    }
    //write PWM values and digital values
    WristRotation.write(wrist_rot_DC);
    WristYaw.write(wrist_yaw_DC);
    Claw.write(claw_DC);       
}

void readSensors(){
    read_sense = buffer[13];
    if(read_sense == 's'){
        //soil.write(0.117f);
        pH = -21.757f * pH_in.read()+17.1175f;
        temp = 165.289f * temp_in.read() -4.921f;
        float x0 = ((3.3f*moist_in.read())/4095);
        moist = -581.12f*(x0*x0) + 691.39f*x0 - 173.53f;
        ser.printf("PH:  %f\t\t", pH);
        ser.printf("TEMP:  %fF\t", temp);
        ser.printf("Moisture:  %f%%\n\r", moist);
        
    }
    else{
        //soil.write(0.097f);
    }
}

//void readGPS(){
////get gps NMEA string and then print it out
////implement a way to detect $GPRMC (where the real data is at)   //add if(buffer[3] == 'R') with gps.gtc() a few times before to get to buffer[3]
////make sure baud rate is at least 9600
////for faster results, try increasing the baud rate
//    read_gps = buffer[14];
//    if(read_gps == 'g'){
//        int done = 0;
//        while(done == 0){
//            //gpsBuffer[0] = gps.getc();
//            if(gpsBuffer[0] == '$'){
//                int i = 1;            
//                while(gpsBuffer[i-1] != '\n'){
//                    gpsBuffer[i] = gps.getc();
//                    i++;
//                    //ser.printf("%d", i);      //debug
//                }
//                ser.printf("Here is the GPS information: %s\n\r", gpsBuffer);
//                done = 1;
//            }
//        }
//    }
//} 

int main(){  
    initWheels();
    //initArm();
    initCamera();
    while(1){
        fillBuffer();
        updateWheels();
        //updateArm();
        //readSensors();
        updateCamera();
        //readSensors();
        //readGPS();
        //ser.printf("done: %d", buffer[1]);
    }
}