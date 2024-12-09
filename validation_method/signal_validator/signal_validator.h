#ifndef SIGNAL_VALIDATOR_H
#define SIGNAL_VALIDATOR_H

#include <vector>
#include "exponential_smoothing/exponential_smoothing.h"
#include "sliding_window/sliding_window.h"

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
    float max_z_score;
};

struct ValidatorParameters
{
    SmoothingParameters smoothing_params;
    WindowParameters window_params;
    float range_l_limit;
    float range_u_limit;
    int invalid_threshold;
};

enum ValidationStatus
{
    VALID           = 0b0,
    INITIALIZING    = 0b10000000,
    MISSING_VALUE   = 0b1000000,
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
    SignalValidator(ValidatorParameters params);
    void update(float new_value);
    float forecast();
    int validate(float value);

private:
    HoltLinearModel smoothing_model;
    SlidingWindow window_model;
    bool is_initializing = true;
    void initialize(float value);

    float range_l_limit;
    float range_u_limit;
    float max_z_score;
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
