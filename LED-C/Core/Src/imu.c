//
// Created by hp on 24-11-2.
//

#include "../Inc/imu.h"
#include "main.h"
#include "spi.h"

extern float ACC[3];
extern float GYRO[3];
uint8_t rxdata_acc[7];
uint8_t rxdata_gyro[6];
uint8_t mode[8];

// 片选
void BMI088_ACCEL_NS_L(void)
{
    HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_RESET);
}
void BMI088_ACCEL_NS_H(void)
{
    HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_SET);
}
void BMI088_GYRO_NS_L(void)
{
    HAL_GPIO_WritePin(CS1_Gyro_GPIO_Port, CS1_Gyro_Pin, GPIO_PIN_RESET);
}
void BMI088_GYRO_NS_H(void)
{
    HAL_GPIO_WritePin(CS1_Gyro_GPIO_Port, CS1_Gyro_Pin, GPIO_PIN_SET);
}

void BMI088_accel_read() {
    BMI088_ACCEL_NS_L();
    BMI088_GYRO_NS_H();
    uint8_t call=0x80 | 0x12;
    HAL_SPI_Transmit(&hspi1, &call, sizeof(call), 1000);
    HAL_SPI_Receive(&hspi1, rxdata_acc, sizeof(rxdata_acc), 1000);
    int16_t ACC_X = ((uint16_t)rxdata_acc[2] << 8) | (uint16_t)rxdata_acc[1];
    int16_t ACC_Y = ((uint16_t)rxdata_acc[4] << 8) | (uint16_t)rxdata_acc[3];
    int16_t ACC_Z = ((uint16_t)rxdata_acc[6] << 8) | (uint16_t)rxdata_acc[5];
    ACC[0] = (float)((int16_t)ACC_X) / 32768.0f * 6.0f;
    ACC[1] = (float)((int16_t)ACC_Y) / 32768.0f * 6.0f;
    ACC[2] = (float)((int16_t)ACC_Z) / 32768.0f * 6.0f;
    BMI088_ACCEL_NS_H();
}

void BMI088_gyro_read() {
    BMI088_GYRO_NS_L();
    BMI088_ACCEL_NS_H();
    uint8_t call=0x80 | 0x02;
    HAL_SPI_Transmit(&hspi1, &call, sizeof(call), 1000);

    HAL_SPI_Receive(&hspi1, rxdata_gyro, sizeof(rxdata_gyro), 1000);
    int16_t GYRO_X = ((uint16_t)rxdata_gyro[1] << 8) | (uint16_t)rxdata_gyro[0];
    int16_t GYRO_Y = ((uint16_t)rxdata_gyro[3] << 8) | (uint16_t)rxdata_gyro[2];
    int16_t GYRO_Z = ((uint16_t)rxdata_gyro[5] << 8) | (uint16_t)rxdata_gyro[4];
    GYRO[0] = (float)((int16_t)GYRO_X) / 32768.0f * 2000;
    GYRO[1] = (float)((int16_t)GYRO_Y) / 32768.0f * 2000;
    GYRO[2] = (float)((int16_t)GYRO_Z) / 32768.0f * 2000;
    BMI088_GYRO_NS_H();
}

void BMI088_accel_write() {
    BMI088_ACCEL_NS_L();
    BMI088_GYRO_NS_H();
    uint8_t txdata[2];
    txdata[0]=0x41;
    txdata[1]=0x01;
    HAL_SPI_Transmit(&hspi1, txdata, sizeof(txdata), 1000);

    BMI088_ACCEL_NS_H();
}

void BMI088_WriteReg(uint8_t reg, uint8_t data) {
    uint8_t txdata[2];
    txdata[0]=reg;
    txdata[1]=data;
    HAL_SPI_Transmit(&hspi1, txdata, sizeof(txdata), 1000);
}

void BMI088_ACCEL_MODE() {
    BMI088_ACCEL_NS_L();
    BMI088_GYRO_NS_H();
    uint8_t call=0x80|0x41;
    HAL_SPI_Transmit(&hspi1, &call, sizeof(call), 1000);
    HAL_SPI_Receive(&hspi1, mode, sizeof(mode), 1000);

    BMI088_ACCEL_NS_H();
}

void BMI088_Init(void) {
    // Soft Reset ACCEL
    BMI088_ACCEL_NS_L();
    BMI088_WriteReg(0x7E, 0xB6); // Write 0xB6 to ACC_SOFTRESET(0x7E)
    HAL_Delay(1);
    BMI088_ACCEL_NS_H();

    // Soft Reset GYRO
    BMI088_GYRO_NS_L();
    BMI088_WriteReg(0x14, 0xB6); // Write 0xB6 to GYRO_SOFTRESET(0x14)
    HAL_Delay(30);
    BMI088_GYRO_NS_H();

    // Switch ACCEL to Normal Mode
    BMI088_ACCEL_NS_L();
    HAL_Delay(1);
    BMI088_WriteReg(0x7D, 0x04); // Write 0x04 to ACC_PWR_CTRL(0x7D)
    HAL_Delay(1);
    BMI088_ACCEL_NS_H();
}