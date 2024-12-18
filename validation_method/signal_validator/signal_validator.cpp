#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include "signal_validator.h"

SignalValidator::SignalValidator(ValidatorParameters params)
    : smoothing_model(params.smoothing_params.alpha, params.smoothing_params.beta),
      window_model(params.window_params.window_size),
      range_l_limit(params.range_l_limit),
      range_u_limit(params.range_u_limit),
      max_z_score(params.window_params.max_z_score),
      min_sd(params.window_params.min_sd),
      max_sd(params.window_params.max_sd),
      max_asc(params.smoothing_params.max_asc),
      max_des(params.smoothing_params.max_des) {}

void SignalValidator::update(float new_value) {
    if (is_initializing) {
        initialize(new_value);
        return;
    }
    float prediction = smoothing_model.forecast();
    window_model.update(new_value - prediction);
    smoothing_model.update(new_value);
}

void SignalValidator::initialize(float value) {
    if (smoothing_model.is_initializing()) {
        smoothing_model.initialize(value);
        return;
    }

    if (window_model.is_initializing()) {
        window_model.update(smoothing_model.forecast() - value);
        smoothing_model.update(value);
    }

    if (!window_model.is_initializing()) {
        is_initializing = false;
    }
}

float SignalValidator::forecast() {
    if (is_initializing) {
        return std::numeric_limits<float>::quiet_NaN();
    }

    return smoothing_model.forecast();
}

int SignalValidator::validate(float value) {
    if (is_initializing) {
        return INITIALIZING;
    }

    int status = VALID;
    if (std::isnan(value)) {
        status |= MISSING_VALUE;
        value = forecast();
    }

    range_check(value) ? status |= OUT_OF_RANGE : status;
    continuity_check(value) ? status |= NOT_CONTINUOUS : status;
    stuck_check() ? status |= STUCK : status;
    noise_check() ? status |= NOISY : status;
    increase_check() ? status |= RAPID_INCREASE : status;
    decrease_check() ? status |= RAPID_DECREASE : status;

    return status;
}

bool SignalValidator::range_check(float value) {
    return value > range_u_limit || value < range_l_limit;
}

bool SignalValidator::continuity_check(float value) {
    float me = window_model.get_me();
    float sd = window_model.get_sd();
    if (stuck_check()) {
        sd = max_sd - min_sd;
    }
    float prediction = smoothing_model.forecast();
    float difference = value - prediction;
    return std::abs((me - difference)) / sd >= max_z_score;
}

bool SignalValidator::stuck_check() {
    return window_model.get_sd() < min_sd;
}

bool SignalValidator::noise_check() {
    return window_model.get_sd() > max_sd;
}

bool SignalValidator::increase_check() {
    return smoothing_model.get_trend() > max_asc;
}

bool SignalValidator::decrease_check() {
    return smoothing_model.get_trend() < max_des;
}
