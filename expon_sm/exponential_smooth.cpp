#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include "exponential_smooth.h"

HoltLinearModel::HoltLinearModel(float alpha, float beta)
        : alpha(alpha), beta(beta), level(0.0), trend(0.0), initialized(false), init_count(0) {
            int alpha_init_count = ceil((1 - alpha) * 10);
            int beta_init_count = ceil((1 - beta) * 10);
            init_len = std::max({alpha_init_count, beta_init_count, 3});
        }

void HoltLinearModel::initialize(float value) {
    if (initialized) {
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

    if (init_count == init_len) {
        initialized = true;
    }
}

// Update the model with a new observation and return predicted next value
void HoltLinearModel::update(float new_value) {
    if (!initialized) {
        std::cerr << "Model not initialized!" << std::endl;
        return;
    }

    float previous_level = level;
    level = alpha * new_value + (1 - alpha) * (level + trend);
    trend = beta * (level - previous_level) + (1 - beta) * trend;
}

float HoltLinearModel::forecast() {
    return level + trend;
}

bool HoltLinearModel::isInitializing() {
    return !initialized;
}

// Function to calculate Mean Square Error (MSE)
float calculate_mse(const std::vector<float> &actual, const std::vector<float> &predicted) {
    float error_sum = 0.0;
    for (size_t i = 0; i < actual.size(); ++i) {
        error_sum += (actual[i] - predicted[i]) * (actual[i] - predicted[i]);
    }
    return error_sum / actual.size();
}

// Function to find the optimal alpha and beta by testing different combinations
void find_optimal_parameters(const std::vector<float> &sensor_data, float &best_alpha, float &best_beta) {
    float best_mse = std::numeric_limits<float>::max();  // Start with a large error value
    std::vector<float> predictions;

    for (float alpha = 0.1; alpha <= 0.9; alpha += 0.05) {
        for (float beta = 0.1; beta <= 0.9; beta += 0.05) {
            HoltLinearModel model(alpha, beta);
            size_t i = 0;
            while (model.isInitializing())
            {
                model.initialize(sensor_data[i]);
                i++;
            }

            predictions.clear();

            // Simulate real-time updates with predictions
            for (; i < sensor_data.size(); ++i) {
                model.update(sensor_data[i]);
                float prediction = model.forecast();
                predictions.push_back(prediction);
            }

            // Calculate MAE for the current alpha-beta combination
            float mse = calculate_mse(std::vector<float>(sensor_data.begin() + 11, sensor_data.end()), predictions);

            if (mse < best_mse) {
                best_mse = mse;
                best_alpha = alpha;
                best_beta = beta;
            }
        }
    }
}
