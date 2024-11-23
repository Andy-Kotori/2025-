//
// Created by hp on 24-11-15.
//

#ifndef CPP_HEAD_H
#define CPP_HEAD_H

#ifdef __cplusplus
extern "C" {
#endif

    void receive();
    void init_motor(const float speed, const float angle);
    void receive_motor(int motor_id);
    void transmit_motor(int mode);
    void shutDown(int motor_id);
    void control();

#ifdef __cplusplus
}
#endif

#endif //CPP_HEAD_H
