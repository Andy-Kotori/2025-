//
// Created by hp on 24-10-30.
//

#ifndef PID_H
#define PID_H

// PID类
class PID {
public:
    PID(float kp, float ki, float kd, float i_max, float out_max);
    float calc(float ref, float fdb);
    void reset();
private:
    float kp_, ki_, kd_;
    float i_max_, out_max_;
    float output_;
    float ref_, fdb_;
    float err_, err_sum_, last_err_;
    float pout_, iout_, dout_;
};

#endif //PID_H
