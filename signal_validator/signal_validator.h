#ifndef SIGNAL_VALIDATOR_H
#define SIGNAL_VALIDATOR_H

#include <vector>
#include "../expon_sm/exponential_smooth.h"
#include "../sliding_wind/sliding_window.h"

enum ValidationStatus
{
    VALID = 0b0,
    OUT_OF_BOUNDS = 0b1,
    OUTLIER = 0b10,
    STUCK = 0b100,
    PRECISION_DEG = 0b1000,
    RAPID_CHANGE = 0b10000
};

class SignalValidator
{
public:
    SignalValidator(size_t window_size, float tolerance, std::vector<float> &training_data);
    SignalValidator(size_t window_size, float tolerance, float alpha, float beta, const float threshold);
    int validate(float value);
    void update(float new_value);
    float forecast();

private:
    HoltLinearModel smooth_model;
    SlidingWindow window_model;
    float tolerance;
};

#endif
