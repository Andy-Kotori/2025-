//
// Created by hp on 24-11-2.
//

#ifndef IMU_H
#define IMU_H

void BMI088_ACCEL_NS_L(void);
void BMI088_ACCEL_NS_H(void);
void BMI088_GYRO_NS_L(void);
void BMI088_GYRO_NS_H(void);
void BMI088_accel_read();
void BMI088_gyro_read();
void BMI088_accel_write();
void BMI088_Init(void);
void BMI088_ACCEL_MODE();
void angleSolve_acc(float* acc);
void angleSolve_gyro(float* gyro);

#endif //IMU_H
