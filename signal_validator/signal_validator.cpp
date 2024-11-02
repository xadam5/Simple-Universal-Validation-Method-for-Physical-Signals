#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include "signal_validator.h"
#include "auto_tuning.h"

SignalValidator::SignalValidator(size_t window_size, float tolerance, float alpha, float beta, const float threshold)
    : smooth_model(alpha, beta), window_model(window_size, threshold), tolerance(tolerance) {}

SignalValidator::SignalValidator(size_t window_size, float tolerance, std::vector<float> &training_data)
    : tolerance(tolerance), smooth_model(0, 0), window_model(0, 0)
{
    ModelParameters optimal_param = find_optimal_param(training_data, window_size, tolerance);
    SmoothingParameters smooth_param = optimal_param.smooth_param;
    WindowParameters window_param = optimal_param.window_param;
    smooth_model = HoltLinearModel(smooth_param.alpha, smooth_param.beta);
    window_model = SlidingWindow(window_size, window_param.threshold);
}

int SignalValidator::validate(float value)
{
    float prediction = smooth_model.forecast();
    float difference = value - prediction;
}
