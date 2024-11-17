//
// Created by hp on 24-10-4.
//

#include "main.h"
#include "stm32f4xx_hal_gpio.h"
#include "usart.h"
#include<string.h>
#include "cpp_head.h"
#include "iwdg.h"
#include "can.h"
#include "spi.h"
#include "tim.h"

extern uint8_t rxBuffer[36u];
extern uint8_t txBuffer[10];
extern uint8_t rxData[36u];
CAN_RxHeaderTypeDef Header;
extern float values[4];
extern uint8_t aData[8];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

    if (htim->Instance == TIM6) {

        HAL_IWDG_Refresh(&hiwdg);

        HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
        HAL_UART_Receive_DMA(&huart3, rxBuffer, 18u);


    }
    if (htim->Instance == TIM5) {
        // HAL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin);
        HAL_UART_Receive_DMA(&huart3, rxBuffer, 18u);
    }
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

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//     if (huart == &huart1) {
//
//         memcpy(txBuffer, rxBuffer, sizeof(rxBuffer));
//         HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, SET);
//         // 将接收到的数据发送回去
//         HAL_UART_Transmit_IT(&huart1, txBuffer, sizeof(txBuffer));
//         HAL_UART_Receive_IT(&huart1, rxBuffer, sizeof(rxBuffer));
//     }
// }

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//     if (huart->Instance == USART1) {
//         HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, SET);
//         HAL_UART_Transmit_DMA(&huart1, rxBuffer, sizeof(rxBuffer));
//         // 处理接收到的数据
//         // 可以再次启动DMA接收，以实现持续接收
//         HAL_UART_Receive_DMA(&huart1, rxBuffer, sizeof(rxBuffer));
//     }
// }

// 遥控器DBUS
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart3) {

        HAL_TIM_Base_Start_IT(&htim5);
        // 复制接收到的数据到发送缓冲区
        memcpy(rxData, rxBuffer, sizeof(rxBuffer));
        receive();
        control();
        // HAL_UART_Receive_DMA(&huart3, rxBuffer, 18u);
        // HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);

    }
}

// 报文解析
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

// 接受电机报文
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if (hcan == &hcan1) {

        HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0, &Header, aData);

        if (Header.StdId == 0x205) {
            canRxMsgCallback_v1(aData);
            receive_motor(1);
        } else if (Header.StdId == 0x207) {
            canRxMsgCallback_v1(aData);
            receive_motor(2);
        }

        // sendMotorSpeed(0x200, calc())


    }
}

