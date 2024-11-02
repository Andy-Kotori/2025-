//
// Created by hp on 24-10-30.
//

#include "../Inc/motor.h"
#include <sys/time.h>
#include "remote_head.h"
#include "PID.h"

extern float values[4];

PID spid(10, 0.5, 1, 4000, 4000);
PID ppid(1, 0.01, 0.8, 10, 360);

motor motor(motor::Type_e::M2006, motor::ControlMethod_e::SPEED, &spid, &ppid);

motor::motor(const Type_e &type, const ControlMethod_e& method, PID *spid, PID *ppid) {
    type_ = type;
    method_ = method;
    spid_ = spid;
    ppid_ = ppid;
    if (type_ == M2006) {
        this->ratio_ = 36.f;
    } else if (type_ == M3508) {
        this->ratio_ = 3591.f / 187.f;
    }
    target_angle=target_speed=fdb_angle=fdb_speed=0;
    spid_->reset();
    ppid_->reset();
    I_out = 0;
    I_out_float = 0;
}

void motor::reset() {
    target_angle=target_speed=fdb_angle=fdb_speed=0;
    spid_->reset();
    ppid_->reset();
    I_out = 0;
    I_out_float = 0;
}

void motor::rxCallback() {
    fdb_angle = values[0];
    fdb_speed = values[1];
}

void motor::setAngle(const float &angle) {
    target_angle = angle;
}

void motor::setSpeed(const float &speed) {
    target_speed = speed;
}

void motor::handle() {
    if (method_ == SPEED) {
        I_out_float = spid_->calc(target_speed, fdb_speed);
    } else if (method_ == POSITION_SPEED) {
        setSpeed(ppid_->calc(target_angle, fdb_angle));
        I_out_float = spid_->calc(target_speed, fdb_speed);
    }
    I_out = (int16_t)I_out_float;
}

void init_motor(const float speed, const float angle) {
    motor.setSpeed(speed);
    motor.setAngle(angle);
}

void receive_motor() {
    motor.rxCallback();
}

void transmit_motor() {
    motor.handle();

    CAN_TxHeaderTypeDef TxHeader;
    uint8_t TxData[8] = { 0 };
    uint32_t TxMailbox;

    // 设置CAN标识符
    TxHeader.StdId = 0x200; // 使用0x200或0x1FF
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = 8;

    // 设置目标转矩电流
    TxData[0] = (motor.I_out >> 8) & 0xFF; // 高8位
    TxData[1] = motor.I_out & 0xFF; // 低8位

    // 将消息添加到发送邮箱
    if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
      // 错误处理
      Error_Handler();
    }
}