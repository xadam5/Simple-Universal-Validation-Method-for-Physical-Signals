#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include "signal_validator.h"

SignalValidator::SignalValidator(ValidatorParameters param)
    : smooth_model(param.smooth_param.alpha, param.smooth_param.beta),
      window_model(param.window_param.window_size),
      range_l_limit(param.range_l_limit),
      range_u_limit(param.range_u_limit),
      continuity_threshold(param.window_param.threshold),
      min_sd(param.window_param.min_sd),
      max_sd(param.window_param.max_sd),
      max_asc(param.smooth_param.max_asc),
      max_des(param.smooth_param.max_des) {}

void SignalValidator::update(float new_value)
{
    if (is_initializing)
    {
        initialize(new_value);
        return;
    }
    float prediction = smooth_model.forecast();
    window_model.update(new_value - prediction);
    smooth_model.update(new_value);
}

void SignalValidator::initialize(float value)
{
    if (smooth_model.isInitializing())
    {
        smooth_model.initialize(value);
        return;
    }

    if (window_model.isInitializing())
    {
        window_model.update(smooth_model.forecast() - value);
        smooth_model.update(value);
    }

    if (!window_model.isInitializing())
    {
        is_initializing = false;
    }
}

float SignalValidator::forecast()
{
    if (is_initializing)
    {
        return std::numeric_limits<float>::quiet_NaN();
    }

    return smooth_model.forecast();
}

int SignalValidator::validate(float value)
{
    if (is_initializing)
    {
        return INITIALIZING;
    }

    int status = VALID;

    float prediction = smooth_model.forecast();
    float difference = value - prediction;

    if (!range_check(value)) { status |= OUT_OF_RANGE; }

    if (!continuity_check(value)) { status |= NOT_CONTINUOUS; }

    if (stuck_check()) { status |= STUCK; }

    if (noise_check()) { status |= NOISY; }

    if (increase_check()) { status |= RAPID_INCREASE; }

    if (decrease_check()) { status |= RAPID_DECREASE; }

    return status;
}

bool SignalValidator::range_check(float value)
{
    return value < range_u_limit && value > range_l_limit;
}

bool SignalValidator::continuity_check(float value)
{
    float me = window_model.get_me();
    float sd = window_model.get_sd();
    if (stuck_check())
    {
        sd = max_sd - min_sd;
    }
    float prediction = smooth_model.forecast();
    float difference = value - prediction;
    return std::abs((me - difference)) / sd < continuity_threshold;
}

bool SignalValidator::stuck_check()
{
    return window_model.get_sd() < min_sd;
}

bool SignalValidator::noise_check()
{
    return window_model.get_sd() > max_sd;
}

bool SignalValidator::increase_check()
{
    return smooth_model.get_trend() > max_asc;
}

bool SignalValidator::decrease_check()
{
    return smooth_model.get_trend() < max_des;
}
