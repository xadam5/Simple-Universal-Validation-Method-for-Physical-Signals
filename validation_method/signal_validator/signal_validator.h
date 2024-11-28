#ifndef SIGNAL_VALIDATOR_H
#define SIGNAL_VALIDATOR_H

#include <vector>
#include "expon_sm/exponential_smooth.h"
#include "sliding_wind/sliding_window.h"

struct SmoothingParameters
{
    float alpha;
    float beta;
    float max_asc;
    float max_des;
};

struct WindowParameters
{
    size_t window_size;
    float min_sd;
    float max_sd;
    float threshold;
};

struct ValidatorParameters
{
    SmoothingParameters smooth_param;
    WindowParameters window_param;
    float range_l_limit;
    float range_u_limit;
};

enum ValidationStatus
{
    VALID           = 0b0,
    INITIALIZING    = 0b1000000,
    OUT_OF_RANGE    = 0b100000,
    NOT_CONTINUOUS  = 0b10000,
    STUCK           = 0b1000,
    NOISY           = 0b100,
    RAPID_INCREASE  = 0b10,
    RAPID_DECREASE  = 0b1,
};

class SignalValidator
{
public:
    SignalValidator(ValidatorParameters param);
    void update(float new_value);
    float forecast();
    int validate(float value);

private:
    HoltLinearModel smooth_model;
    SlidingWindow window_model;
    bool is_initializing = true;
    void initialize(float value);

    float range_l_limit;
    float range_u_limit;
    float continuity_threshold;
    float min_sd;
    float max_sd;
    float max_asc;
    float max_des;

    bool range_check(float value);
    bool continuity_check(float value);
    bool stuck_check();
    bool noise_check();
    bool increase_check();
    bool decrease_check();
};

#endif
