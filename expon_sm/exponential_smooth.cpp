#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include "exponential_smooth.h"

HoltLinearModel::HoltLinearModel(float alpha, float beta)
    : alpha(alpha), beta(beta), level(0.0), trend(0.0), initialized(false), init_count(0)
{
    int alpha_init_count = ceil((1 - alpha) * 10);
    int beta_init_count = ceil((1 - beta) * 10);
    init_len = std::max({alpha_init_count, beta_init_count, 3});
}

void HoltLinearModel::initialize(float value)
{
    if (initialized)
    {
        return;
    }

    switch (init_count)
    {
    case 0:
        level = value;
        init_count++;
        break;

    case 1:
        trend = level - value;
        level = value;
        init_count++;
        break;

    default:
        float init_alpha = 1 - (1 - alpha) / init_len * init_count;
        float init_beta = 1 - (1 - beta) / init_len * init_count;
        float previous_level = level;
        level = init_alpha * value + (1 - init_alpha) * (level + trend);
        trend = init_beta * (level - previous_level) + (1 - init_beta) * trend;
        init_count++;
        break;
    }

    if (init_count == init_len)
    {
        initialized = true;
    }
}

// Update the model with a new observation and return predicted next value
void HoltLinearModel::update(float new_value)
{
    if (!initialized)
    {
        std::cerr << "Model not initialized!" << std::endl;
        return;
    }

    float previous_level = level;
    level = alpha * new_value + (1 - alpha) * (level + trend);
    trend = beta * (level - previous_level) + (1 - beta) * trend;
}

float HoltLinearModel::forecast()
{
    return level + trend;
}

bool HoltLinearModel::isInitializing()
{
    return !initialized;
}

float HoltLinearModel::get_level()
{
    return level;
}
