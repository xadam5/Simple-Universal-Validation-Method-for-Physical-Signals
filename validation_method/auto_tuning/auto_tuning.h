#ifndef AUTO_TUNER_H
#define AUTO_TUNER_H

#include <limits>
#include "../signal_validator/signal_validator.h"

struct TunnedParameters
{
    SmoothingParameters smooth_param;
    WindowParameters window_param;
};

TunnedParameters find_optimal_param(std::vector<float> &tuning_data, size_t wintdo_size, float tolerance);

#endif
