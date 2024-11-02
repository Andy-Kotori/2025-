//
// Created by hp on 24-10-30.
//

#ifndef MOTOR_H
#define MOTOR_H

#include "can.h"
#include "PID.h"

class motor {
public:

    int16_t I_out;

    // 电机型号
    typedef enum Type {
        // DJI motor
        M3508,
        M2006,
      } Type_e;

    typedef enum ControlMethod {
        POSITION_SPEED,  // 位置+速度控制
        // POSITION,        // 位置控制
        SPEED,           // 速度控制
        // TORQUE,          // 力矩控制
      } ControlMethod_e;

    motor(const Type_e& type, const ControlMethod_e& method, PID* spid, PID* ppid);
    void reset();
    void rxCallback();
    void setAngle(const float& angle);
    void setSpeed(const float& speed);
    void handle();


private:
    Type_e type_;   // 型号
    ControlMethod_e method_;
    float ratio_;
    PID* spid_;
    PID* ppid_;

    // control data 电机控制数据(目标&反馈)
    // unit: deg, ras/s, (N*m or none)
    // target 目标值
    float target_angle, target_speed;
    // feedback 反馈值
    float fdb_angle, fdb_speed;

    float I_out_float;


};

#endif //MOTOR_H
