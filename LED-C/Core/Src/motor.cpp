//
// Created by hp on 24-11-15.
//

#include "../Inc/motor.h"

// #include <sys/time.h>
#include <cpp_head.h>
#include "PID.h"

extern float values[4];
extern motor motor_p;
extern motor motor_r;

motor::motor(const Type_e &type, const ControlMethod_e& method, PID *spid, PID *ppid) {
    type_ = type;
    method_ = method;
    spid_ = spid;
    ppid_ = ppid;
    if (type_ == M2006) {
        this->ratio_ = 36.f;
    } else if (type_ == M3508) {
        this->ratio_ = 3591.f / 187.f;
    } else if (type_ == M6002) {
        this->ratio_ = 1.f;
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
    fdb_angle = values[0]; // motor fdb control
    fdb_speed = values[1]; // motor fdb control
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
        if (target_angle - fdb_angle > 180) {
            setSpeed(ppid_->calc(target_angle, fdb_angle+360));
        } else if (target_angle - fdb_angle < -180) {
            setSpeed(ppid_->calc(target_angle, fdb_angle-360));
        } else {
            setSpeed(ppid_->calc(target_angle, fdb_angle));
        }
        I_out_float = spid_->calc(target_speed, fdb_speed);
    }
    I_out = (int16_t)I_out_float;
}

void init_motor(int motor_id, const float speed, const float angle) {
    if (motor_id == 1) {
        motor_p.setSpeed(speed);
        motor_p.setAngle(angle);
    }
}

void receive_motor(int motor_id) {
    if (motor_id == 1) {
        motor_p.rxCallback();
    } else if (motor_id == 2) {
        motor_r.rxCallback();
    }
}

void transmit_motor(int mode) {

    if (mode == 0) {
        motor_r.handle();
    } else if (mode == 1) {
        motor_p.handle();
        motor_r.handle();
    }

    CAN_TxHeaderTypeDef TxHeader;
    uint8_t TxData[8] = { 0 };
    uint32_t TxMailbox;

    // 设置CAN标识符
    TxHeader.StdId = 0x1FF; // 使用0x200或0x1FF
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = 8;

    TxData[0] = (motor_p.I_out >> 8) & 0xFF; // 高8位
    TxData[1] = motor_p.I_out & 0xFF; // 低8位
    TxData[4] = (motor_r.I_out >> 8) & 0xFF; // 高8位
    TxData[5] = motor_r.I_out & 0xFF; // 低8位

    // 将消息添加到发送邮箱
    if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
        // 错误处理
        Error_Handler();
    }
}

void shutDown(int motor_id) {
    if (motor_id == 1) {
        motor_p.reset();

        CAN_TxHeaderTypeDef TxHeader;
        uint8_t TxData[8] = { 0 };
        uint32_t TxMailbox;

        // 设置CAN标识符
        TxHeader.StdId = 0x1FF; // 使用0x200或0x1FF
        TxHeader.IDE = CAN_ID_STD;
        TxHeader.RTR = CAN_RTR_DATA;
        TxHeader.DLC = 8;

        // 设置目标转矩电流
        TxData[0] = 0; // 高8位
        TxData[1] = 0; // 低8位

        // 将消息添加到发送邮箱
        if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
            // 错误处理
            Error_Handler();
        }
    } else if (motor_id == 2) {
        motor_r.reset();

        CAN_TxHeaderTypeDef TxHeader;
        uint8_t TxData[8] = { 0 };
        uint32_t TxMailbox;

        // 设置CAN标识符
        TxHeader.StdId = 0x1FF; // 使用0x200或0x1FF
        TxHeader.IDE = CAN_ID_STD;
        TxHeader.RTR = CAN_RTR_DATA;
        TxHeader.DLC = 8;

        // 设置目标转矩电流
        TxData[4] = 0; // 高8位
        TxData[5] = 0; // 低8位

        // 将消息添加到发送邮箱
        if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
            // 错误处理
            Error_Handler();
        }
    }
}