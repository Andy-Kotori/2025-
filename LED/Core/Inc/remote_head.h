//
// Created by hp on 24-10-10.
//

#ifndef REMOTE_HEAD_H
#define REMOTE_HEAD_H


#ifdef __cplusplus
extern "C" {
#endif

    void receive();
    void init_motor(const float speed, const float angle);
    void receive_motor();
    void transmit_motor();

#ifdef __cplusplus
}
#endif




#endif //REMOTE_HEAD_H
