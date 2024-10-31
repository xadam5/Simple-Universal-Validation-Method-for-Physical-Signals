#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include "signal_validator.h"


struct window_param {
    float min_sd = 0;
    float max_sd = std::numeric_limits<float>::max();
    float threshold = 0;
    int window_size = 0;
};


window_param tune_window_size(std::vector<float> training_data, float alpha, float beta) {
    if (training_data.size() < 40) {
        std::cerr << "Training dataset is too small for autotuning, setting to default window size" << std:: endl;
    }

    window_param best_params;

    std::vector<int> window_sizes = {5, 8, 10, 12, 15, 18, 20};
    for (int window_size : window_sizes) {
        HoltLinearModel smooth_model = HoltLinearModel();
        smooth_model.configure(alpha, beta, 10);
        int i = 0;
        while (smooth_model.isInitializing()) {
            smooth_model.initialize(training_data[i]);
            i++;
        }

        SlidingWindow window_model = SlidingWindow(window_size, 0);
        while (window_model.isInitializing()) {
            float prediction = smooth_model.forecast();
            window_model.update(training_data[i] - prediction);
            smooth_model.update(training_data[i]);
            i++;
        }
        
        float min_sd = std::numeric_limits<float>::max();
        float max_sd = 0;
        float max_sd_from_me = 0;

        for (; i < training_data.size(); i++) {
            float prediction = smooth_model.forecast();
            float real_value = training_data[i];
            float difference = real_value - prediction;
            window_model.update(difference);
            float sd = window_model.standard_deviation();
            if (sd > max_sd) {
                max_sd = sd;
            }

            if (sd < min_sd) {
                min_sd = sd;
            }
            float me = window_model.mean();
            float sd_from_me = std::abs(me - difference) / sd;
            if (sd_from_me > max_sd_from_me) {
                max_sd_from_me = sd_from_me;
            }

            smooth_model.update(real_value);
        }

        std::cout << "Window size: " << window_size << "    Max sd: " << max_sd << "    Min sd: " << min_sd << "    Max num of sd from me: " << max_sd_from_me << std::endl;
        if (best_params.max_sd - best_params.min_sd > max_sd - min_sd) {
            best_params.max_sd = max_sd;
            best_params.min_sd = min_sd;
            best_params.threshold = max_sd_from_me * 1.4;
            best_params.window_size = window_size;
        }

    }

    return best_params;
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
