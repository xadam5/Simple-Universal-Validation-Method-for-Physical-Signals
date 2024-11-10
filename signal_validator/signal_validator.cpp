#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include "signal_validator.h"
#include "auto_tuning.h"

SignalValidator::SignalValidator(size_t window_size, float tolerance, float alpha, float beta, float threshold)
    : smooth_model(alpha, beta), window_model(window_size, threshold), tolerance(tolerance) {}

int SignalValidator::validate(float value)
{
    float prediction = smooth_model.forecast();
    float difference = value - prediction;
}
