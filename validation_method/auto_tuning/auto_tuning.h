#ifndef AUTO_TUNER_H
#define AUTO_TUNER_H

#include <limits>
#include "../signal_validator/signal_validator.h"

struct TunnedParameters
{
    SmoothingParameters smoothing_params;
    WindowParameters window_params;
};

TunnedParameters find_optimal_params(std::vector<float> &tuning_data, size_t wintdo_size, float tolerance);

#endif
