//
// Created by hp on 24-11-16.
//

#include "../Inc/control.h"
#include<cpp_head.h>
#include "remote.h"
#include "motor.h"
#include "can.h"
#include "PID.h"

extern float remote_output[6];
float Amplitude_p = 25;
float Offset_p = 227;
float Amplitude_r = 60;
float Offset_r = 65;
float Offset_p_imu = 0;

PID spid_p(50, 0, 0, 4000, 4000);
PID ppid_p(30, 1.25, 30, 10, 360);
// PID spid_p(25, 0, 0, 4000, 4000); // imu
// PID ppid_p(40, 2.3, 15, 20, 8000); // imu
PID spid_r(50, 0.0, 0, 4000, 25000);
PID ppid_r(25, 3, 50, 8, 8000);
motor motor_p(motor::Type_e::M6002, motor::ControlMethod_e::POSITION_SPEED, &spid_p, &ppid_p);
motor motor_r(motor::Type_e::M6002, motor::ControlMethod_e::POSITION_SPEED, &spid_r, &ppid_r);


int16_t feedFoward(float angle) {
    return (int16_t)angle;
}

void control() {
    if (remote_output[4] == RC_SW_UP) {
        if (remote_output[5] == RC_SW_DOWN) {
            shutDown(1);
            shutDown(2);
        } else if (remote_output[5] == RC_SW_UP) { // Normal PID
            float remote_input_p = remote_output[3];
            float target_angle_p = Amplitude_p * remote_input_p + Offset_p;
            motor_p.setAngle(target_angle_p);
            float remote_input_r = remote_output[2];
            float target_angle_r = Amplitude_r * remote_input_r + Offset_r;
            motor_r.setAngle(target_angle_r);
            transmit_motor(1);
        } else if (remote_output[5] == RC_SW_MID) { // Gravity Compensation
            // float remote_input_r = remote_output[2];
            // float target_angle_r = Amplitude_r * remote_input_r + Offset_r;
            // motor_r.setAngle(target_angle_r);
            // motor_p.I_out = feedFoward(motor_p.fdb_angle);
            // transmit_motor(0);
        }
    } else if (remote_output[4] == RC_SW_MID) {
        if (remote_output[5] == RC_SW_DOWN) {
            shutDown(1);
            shutDown(2);
        } else if (remote_output[5] == RC_SW_UP) { // Using ACC & GYRO
            float remote_input_p = remote_output[3];
            float target_angle_p = Amplitude_p * remote_input_p + Offset_p_imu;
            motor_p.setAngle(target_angle_p);
            transmit_motor(2);
        }
    }
}
