//
// Created by hp on 24-10-3.
//
#include "gpio.h"
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "stm32f4xx_hal.h"
#include<string.h>

extern uint8_t rxBuffer[15];
extern uint8_t txBuffer[15];

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
        HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, RESET);
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



