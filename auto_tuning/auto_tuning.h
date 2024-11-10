#ifndef AUTO_TUNER_H
#define AUTO_TUNER_H

#include <limits>
#include "../expon_sm/exponential_smooth.h"
#include "../sliding_wind/sliding_window.h"

struct SmoothingParameters
{
    float alpha;
    float beta;
    float max_change;
};

struct WindowParameters
{
    float min_sd;
    float max_sd;
    float threshold;
};

struct ModelParameters
{
    SmoothingParameters smooth_param;
    WindowParameters window_param;
};

ModelParameters find_optimal_param(std::vector<float> &training_data, size_t wintdo_size, float tolerance);

#endif
