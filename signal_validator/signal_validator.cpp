#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include "signal_validator.h"
#include "auto_tuning.h"

SignalValidator::SignalValidator(const float alpha, const float beta, const size_t window_size, const float threshold, const float tolerance)
: smooth_model(alpha, beta), window_model(window_size, threshold) {}

SignalValidator::SignalValidator(const std::vector<float> &training_data) : smooth_model(0, 0), window_model(0, 0) {
    ModelParameters optimal_param = find_optimal_param(training_data);
    SmoothingParameters smooth_param = optimal_param.smooth_param;
    WindowParameters window_param = optimal_param.window_param;
    smooth_model = HoltLinearModel(smooth_param.alpha, smooth_param.beta);
    window_model = SlidingWindow(window_param.window_size, window_param.threshold);
}
