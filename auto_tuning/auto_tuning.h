#ifndef AUTO_TUNER_H
#define AUTO_TUNER_H

#include <limits>
#include "../expon_sm/exponential_smooth.h"
#include "../sliding_wind/sliding_window.h"

struct smoothing_param {
    float alpha;
    float beta;
};

struct window_param {
    float min_sd;
    float max_sd;
    float threshold;
    int window_size;
};

struct model_param {
    smoothing_param smooth_model;
    window_param window_model;
};

model_param find_optimal_param(const std::vector<float> &training_data);

#endif
