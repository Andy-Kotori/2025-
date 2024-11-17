//
// Created by hp on 24-11-15.
//

#include "../Inc/remote.h"
#include<cpp_head.h>
#include<math.h>
// #include "stm32f4xx_hal_uart.h"
// #include "usart.h"
/* ----------------------- Internal Data ----------------------------------- */
volatile unsigned char sbus_rx_buffer[2][RC_FRAME_LENGTH]; //double sbus rx buffer to save data
static RC_Ctl_t RC_CtrlData;

extern uint8_t rxData[36u];
extern float remote_output[6];

float Normalization_ch(uint16_t data) {
    uint16_t min=RC_CH_VALUE_MIN;
    uint16_t max=RC_CH_VALUE_MAX;
    uint16_t range=max-min;
    return 2*((float)(data-RC_CH_VALUE_OFFSET))/range;
}

void RC_Ctl_t::RemoteDataProcess(uint8_t *pData)
{
    if(pData == nullptr)
    {
        return;
    }

    rc.ch0 = ((int16_t)pData[0] | ((int16_t)pData[1] << 8)) & 0x07FF;
    rc.ch1 = (((int16_t)pData[1] >> 3) | ((int16_t)pData[2] << 5))
   & 0x07FF;
    rc.ch2 = (((int16_t)pData[2] >> 6) | ((int16_t)pData[3] << 2) |
    ((int16_t)pData[4] << 10)) & 0x07FF;
    rc.ch3 = (((int16_t)pData[4] >> 1) | ((int16_t)pData[5]<<7)) &
   0x07FF;

    rc.s1 = ((pData[5] >> 4) & 0x000C) >> 2;
    rc.s2 = ((pData[5] >> 4) & 0x0003);
    mouse.x = ((int16_t)pData[6]) | ((int16_t)pData[7] << 8);
    mouse.y = ((int16_t)pData[8]) | ((int16_t)pData[9] << 8);
    mouse.z = ((int16_t)pData[10]) | ((int16_t)pData[11] << 8);
    mouse.press_l = pData[12];
    mouse.press_r = pData[13];
    key.v = ((int16_t)pData[14]);// | ((int16_t)pData[15] << 8);
    solved_data_.ch0=remote_output[0]=Normalization_ch(rc.ch0);
    solved_data_.ch1=remote_output[1]=Normalization_ch(rc.ch1);
    solved_data_.ch2=remote_output[2]=Normalization_ch(rc.ch2);
    solved_data_.ch3=remote_output[3]=Normalization_ch(rc.ch3);
    if (rc.s1 == RC_SW_UP) {
        solved_data_.s1=UP;
        remote_output[4]=RC_SW_UP;
    } else if (rc.s1 == RC_SW_MID) {
        solved_data_.s1=MID;
        remote_output[4]=RC_SW_MID;
    } else if (rc.s1 == RC_SW_DOWN) {
        solved_data_.s1=DOWN;
        remote_output[4]=RC_SW_DOWN;
    }
    if (rc.s2 == RC_SW_UP) {
        solved_data_.s2=UP;
        remote_output[5]=RC_SW_UP;
    } else if (rc.s2 == RC_SW_MID) {
        solved_data_.s2=MID;
        remote_output[5]=RC_SW_MID;
    } else if (rc.s2 == RC_SW_DOWN) {
        solved_data_.s2=DOWN;
        remote_output[5]=RC_SW_DOWN;
    }
}

void receive() {
    RC_CtrlData.RemoteDataProcess(rxData);
}

// void transmit() {
//     HAL_UART_Transmit_IT(&huart6, RC_CtrlData.output, sizeof(RC_CtrlData.output));
// }