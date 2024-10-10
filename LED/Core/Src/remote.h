//
// Created by hp on 24-10-9.
//

#ifndef REMOTE_H
#define REMOTE_H

#include "stdint.h"

/* ----------------------- RC Channel Definition---------------------------- */
#define RC_CH_VALUE_MIN ((uint16_t)364 )
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)
/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01<<7)
#define RC_FRAME_LENGTH 18u
/* ----------------------- Data Struct ------------------------------------- */
struct RC_Ctl_t
{
public:
    struct
    {
        uint16_t ch0=-1;
        uint16_t ch1=-1;
        uint16_t ch2=-1;
        uint16_t ch3=-1;
        uint8_t s1=-1;
        uint8_t s2=-1;
    }rc;
    struct
    {
        int16_t x=-1;
        int16_t y=-1;
        int16_t z=-1;
        uint8_t press_l=-1;
        uint8_t press_r=-1;
    }mouse;
    struct
    {
        uint16_t v=-1;
    }key;
    float output[6];
    void RemoteDataProcess(uint8_t *pData);
private:
    typedef enum RCSwitchState {
        UP,
        MID,
        DOWN,
        NONE
      } RCSwitchState_e;
    struct Solved_data
    {
        float ch0=-1;
        float ch1=-1;
        float ch2=-1;
        float ch3=-1;
        RCSwitchState_e s1=NONE;
        RCSwitchState_e s2=NONE;
    }solved_data_;

    void Solve_Raw_Data();
};



#endif //REMOTE_H
