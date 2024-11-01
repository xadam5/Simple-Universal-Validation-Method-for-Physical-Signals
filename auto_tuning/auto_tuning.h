#ifndef AUTO_TUNER_H
#define AUTO_TUNER_H

#include <limits>
#include "exponential_smooth.h"
#include "sliding_window.h"

struct SmoothingParameters {
    float alpha;
    float beta;
};

struct WindowParameters {
    float min_sd;
    float max_sd;
    float threshold;
    int window_size;
};

struct ModelParameters {
    SmoothingParameters smooth_param;
    WindowParameters window_param;
};

ModelParameters find_optimal_param(const std::vector<float> &training_data);

#endif
