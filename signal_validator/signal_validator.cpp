#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include "signal_validator.h"
#include "auto_tuning.h"

SignalValidator::SignalValidator(ValidatorParameters param)
    : smooth_model(param.smooth_param.alpha, param.smooth_param.beta),
      window_model(param.window_param.window_size),
      range_l_limit(param.range_l_limit),
      range_u_limit(param.range_u_limit),
      continuity_threshold(param.continuity_threshold),
      min_sd(param.window_param.min_sd),
      max_sd(param.window_param.max_sd),
      max_asc(param.smooth_param.max_asc),
      max_des(param.smooth_param.max_des) {}

void SignalValidator::update(float new_value)
{
    float prediction = smooth_model.forecast();
    window_model.update(new_value - prediction);
    smooth_model.update(new_value);
}

float SignalValidator::forecast()
{
    return smooth_model.forecast();
}

int SignalValidator::validate(float value)
{
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
    float prediction = smooth_model.forecast();
    float difference = value - prediction;
    return (window_model.get_me() - difference) / window_model.get_sd() < continuity_threshold;
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
