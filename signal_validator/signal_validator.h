#ifndef SIGNAL_VALIDATOR_H
#define SIGNAL_VALIDATOR_H

#include <vector>
#include "../expon_sm/exponential_smooth.h"
#include "../sliding_wind/sliding_window.h"

class SignalValidator {
public:
    SignalValidator(const std::vector<float> &training_data);
    SignalValidator(const float alpha, const float beta, const size_t window_size, const float threshold, const float tolerance);
    char validate(float value);
    void update(float new_value);
    float forecast();

private:
    HoltLinearModel smooth_model;
    SlidingWindow window_model;
};

#endif
