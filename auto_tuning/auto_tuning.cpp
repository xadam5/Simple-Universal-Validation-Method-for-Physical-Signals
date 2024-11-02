#include <vector>
#include <cmath>
#include <limits>
#include "auto_tuning.h"

// Function to calculate Mean Square Error (MSE)
float calculate_mse(const std::vector<float> &actual, const std::vector<float> &predicted)
{
    float error_sum = 0.0;
    for (size_t i = 0; i < actual.size(); ++i)
    {
        error_sum += (actual[i] - predicted[i]) * (actual[i] - predicted[i]);
    }
    return error_sum / actual.size();
}

// Function to find the optimal alpha and beta by testing different combinations
SmoothingParameters tune_smooth_param(const std::vector<float> &training_data)
{
    float best_mse = std::numeric_limits<float>::max();
    float best_alpha, best_beta, best_max_change;
    std::vector<float> predictions;

    for (float alpha = 0.1; alpha <= 0.9; alpha += 0.05)
    {
        for (float beta = 0.1; beta <= 0.9; beta += 0.05)
        {
            HoltLinearModel model(alpha, beta);
            size_t i = 0;
            while (model.isInitializing())
            {
                model.initialize(training_data[i]);
                i++;
            }

            predictions.clear();
            float max_change = 0;

            // Simulate real-time updates with predictions
            for (; i < training_data.size(); ++i)
            {
                float prediction = model.forecast();
                predictions.push_back(prediction);
                model.update(training_data[i]);
                max_change = std::max(max_change, model.get_level());
            }

            // Calculate MSE for the current alpha-beta combination
            size_t init_size = training_data.size() - predictions.size();
            float mse = calculate_mse(std::vector<float>(training_data.begin() + init_size, training_data.end()), predictions);

            if (mse < best_mse)
            {
                best_mse = mse;
                best_alpha = alpha;
                best_beta = beta;
                best_max_change = max_change;
            }
        }
    }

    return SmoothingParameters(best_alpha, best_beta, best_max_change);
}

WindowParameters tune_window_param(const std::vector<float> &training_data, size_t window_size, float tolerance, SmoothingParameters smooth_param)
{
    WindowParameters best_params(0, std::numeric_limits<float>::max(), 0);
    HoltLinearModel smooth_model = HoltLinearModel(smooth_param.alpha, smooth_param.beta);
    int i = 0;
    while (smooth_model.isInitializing())
    {
        smooth_model.initialize(training_data[i]);
        i++;
    }

    SlidingWindow window_model = SlidingWindow(window_size, 0);
    while (window_model.isInitializing())
    {
        float prediction = smooth_model.forecast();
        window_model.update(training_data[i] - prediction);
        smooth_model.update(training_data[i]);
        i++;
    }

    float min_sd = std::numeric_limits<float>::max();
    float max_sd = 0;
    float max_sd_from_me = 0;
    float min_st_from_me = std::numeric_limits<float>::max();

    for (; i < training_data.size(); i++)
    {
        float prediction = smooth_model.forecast();
        float real_value = training_data[i];
        float difference = real_value - prediction;
        window_model.update(difference);
        float sd = window_model.standard_deviation();

        max_sd = std::max(max_sd, sd);
        min_sd = std::min(min_sd, sd);

        float me = window_model.mean();
        float sd_from_me = std::abs(me - difference) / sd;
        max_sd_from_me = std::max(max_sd_from_me, sd_from_me);
        min_st_from_me = std::min(min_st_from_me, sd_from_me);

        smooth_model.update(real_value);
    }

    best_params.max_sd = max_sd;
    best_params.min_sd = min_sd;
    best_params.threshold = max_sd_from_me + tolerance * (max_sd_from_me - min_st_from_me);

    return best_params;
}

ModelParameters find_optimal_param(const std::vector<float> &training_data, size_t window_size, float tolerance)
{
    SmoothingParameters smooth_model = tune_smooth_param(training_data);
    WindowParameters window_model = tune_window_param(training_data, window_size, tolerance, smooth_model);
    return ModelParameters(smooth_model, window_model);
}
