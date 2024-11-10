#ifndef AUTO_TUNER_H
#define AUTO_TUNER_H

#include <limits>
#include "../expon_sm/exponential_smooth.h"
#include "../sliding_wind/sliding_window.h"
#include "../signal_validator/signal_validator.h"

ValidatorParameters find_optimal_param(std::vector<float> &training_data, size_t wintdo_size, float tolerance);

#endif
