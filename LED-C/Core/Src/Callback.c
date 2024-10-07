//
// Created by hp on 24-10-4.
//

#include "main.h"
#include "stm32f4xx_hal_gpio.h"
#include "usart.h"
#include<string.h>

extern uint8_t rxBuffer[10];
extern uint8_t txBuffer[10];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

    if (htim->Instance == TIM6) {
        HAL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin);
    }
    // if (htim->Instance == TIM5) {
    //     HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
    // }
}

// void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef* htim) {
//     if (htim->Instance == TIM5) {
//         HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
//     }
// }

// void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
//     // HAL_Delay(100);
//     if (huart == &huart1) {
//         HAL_UART_Transmit_IT(huart, (uint8_t *)"HelloWorld", 10);
//     }
// }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart1) {

        memcpy(txBuffer, rxBuffer, sizeof(rxBuffer));
        HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, SET);
        // 将接收到的数据发送回去
        HAL_UART_Transmit_IT(&huart1, txBuffer, sizeof(txBuffer));
        HAL_UART_Receive_IT(&huart1, rxBuffer, sizeof(rxBuffer));
    }
}
