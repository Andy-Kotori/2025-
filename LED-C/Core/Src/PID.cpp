//
// Created by hp on 24-11-15.
//

#include "../Inc/PID.h"
#include "math.h"

float limit(float out, float limit) {
    limit = fabs(limit);
    if (fabs(out) <= limit) {
        return out;
    } else {
        if (out>=0) {
            return limit;
        } else {
            return -limit;
        }
    }
}

PID::PID(float kp, float ki, float kd, float i_max, float out_max) {
    this->kp_ = kp;
    this->ki_ = ki;
    this->kd_ = kd;
    this->i_max_ = i_max;
    this->out_max_ = out_max;
    this->output_ = ref_ = fdb_ = err_ = err_sum_ = last_err_ = pout_ = iout_ = dout_ = 0;
}

float PID::calc(float ref, float fdb) {
    this->ref_ = ref;
    this->fdb_ = fdb;
    this->err_ = ref_ - fdb_;
    this->err_sum_=limit(err_sum_+err_, i_max_);
    this->pout_ = kp_ * err_;
    this->iout_ = ki_ * err_sum_;
    this->dout_ = kd_ * (last_err_ - err_);
    this->output_ = limit(pout_ + iout_ - dout_, out_max_);
    this->last_err_ = err_;
    return output_;
}

void PID::reset() {
    output_ = ref_ = fdb_ = err_ = err_sum_ = last_err_ = pout_ = iout_ = dout_ = 0;
}
