//
// Created by hp on 24-10-3.
//
#include <can.h>

#include "gpio.h"
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "stm32f4xx_hal.h"
#include<string.h>
#include "../Inc/remote_head.h"

extern uint8_t rxBuffer[36u];
extern uint8_t rx[15];
extern uint8_t rxData[36u];
extern uint8_t aData[8];
uint8_t Data;
CAN_RxHeaderTypeDef Header;
float values[4];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // if (htim->Instance == TIM1 && flag == 0) {
    //   HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
    //   HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
    //   flag=1;
    // }
    // else if (htim->Instance == TIM1 && flag == 1) {
    //   HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
    //   HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
    //   flag=0;
    // }
    if (htim->Instance == TIM1) {
        HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
        HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
    }
}


// void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
//     // HAL_Delay(100);
//     if (huart == &huart6) {
//         HAL_UART_Transmit_IT(huart, (uint8_t *)"HelloWorld", 10);
//     }
// }

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//     if (huart == &huart6) {
//         // 将接收到的数据发送回去
//         HAL_UART_Transmit(&huart6, rxBuffer, sizeof(rxBuffer), HAL_MAX_DELAY);
//         HAL_UART_Receive_IT(&huart6, rxBuffer, sizeof(rxBuffer));
//     }
// }


// // UART转发点灯
// // UART接收完成回调函数
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//     if (huart == &huart6) {
//         // 点亮红灯
//         HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, SET);
//
//         // 将接收到的数据复制到发送数组
//         memcpy(txBuffer, rxBuffer, sizeof(rxBuffer));
//
//         // 发送数据
//         HAL_UART_Transmit_IT(&huart6, txBuffer, sizeof(txBuffer));
//         HAL_UART_Receive_IT(&huart6, rxBuffer, sizeof(rxBuffer));
//     }
// }

// //DMA
// // UART接收完成中断服务例程
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//     if (huart == &huart6) {
//
//         // 复制接收到的数据到发送缓冲区
//         memcpy(txBuffer, rxBuffer, sizeof(rxBuffer));
//
//         HAL_TIM_Base_Start_IT(&htim1);
//
//         // 通过DMA发送数据
//         HAL_UART_Transmit_DMA(&huart6, txBuffer, sizeof(txBuffer));
//         HAL_UART_Receive_DMA(&huart6, rxBuffer, sizeof(rxBuffer));
//
//
//         HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, SET);
//
//     }
// }

// // 遥控器DBUS
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//     if (huart == &huart1) {
//
//         HAL_TIM_Base_Start_IT(&htim1);
//         // 复制接收到的数据到发送缓冲区
//         memcpy(rxData, rxBuffer, sizeof(rxBuffer));
//         receive();
//         HAL_UART_Receive_DMA(&huart1, rxBuffer, 18u);
//
//         // HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
//
//     }
//
//     if (huart == &huart6) {
//         // HAL_TIM_Base_Start_IT(&htim1);
//         HAL_UART_Transmit_DMA(&huart6, rx, sizeof(rx));
//         HAL_UART_Receive_DMA(&huart6, rx, sizeof(rx));
//     }
// }

float linearMapping(uint16_t in, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max){
    float k=((float)(out_max-out_min))/((float)(in_max-in_min));    // 计算斜率
    return (float)(out_min)+k*((float)(in-in_min));
}

void canRxMsgCallback_v1(uint8_t rx_data[8]) {
    uint16_t ecd = (uint16_t)rx_data[0] << 8 | (uint16_t)rx_data[1];
    float ecd_angle_ = values[0] = linearMapping(ecd, 0, 8191, 0, 360);
    float rotate_speed_ = values[1] = (float)(int16_t)((uint16_t)rx_data[2] << 8 | (uint16_t)rx_data[3]);
    float current_ = values[2] = (float)(int16_t)((uint16_t)rx_data[4] << 8 | (uint16_t)rx_data[5]);
    // float temp_ = values[3] = (float)(int8_t)rx_data[6];
}

void sendMotorSpeed(uint32_t id, int16_t current) {
    CAN_TxHeaderTypeDef TxHeader;
    uint8_t TxData[8] = { 0 };
    uint32_t TxMailbox;

    // 设置CAN标识符
    TxHeader.StdId = id; // 使用0x200或0x1FF
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = 8;

    // 设置目标转矩电流
    TxData[0] = (current >> 8) & 0xFF; // 高8位
    TxData[1] = current & 0xFF; // 低8位

    // 将消息添加到发送邮箱
    if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
        // 错误处理
        Error_Handler();
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if (hcan == &hcan1) {
        HAL_TIM_Base_Start_IT(&htim1);

        HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0, &Header, aData);

        // if (Header.StdId == 0x000) {
        //     // HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, Data ? SET : RESET);
        //     HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
        // }

        canRxMsgCallback_v1(aData);
        // sendMotorSpeed(0x200, calc())
        receive_motor();

    }
}

int flag=0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    CAN_TxHeaderTypeDef TxHeader;
    if (flag==1) {
        flag=0;
    } else {
        flag=1;
    }
    uint8_t TxData = flag;
    uint32_t TxMailbox;

    // 设置CAN标识符
    TxHeader.StdId = 0x000; // 使用0x200或0x1FF
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = 1;

    HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &TxData, &TxMailbox);
}



