#include <vector>
#include <cmath>
#include <limits>
#include "auto_tuning.h"

void initialize_HoltLinearModel(HoltLinearModel &smooth_model, const std::vector<float> &training_data, size_t &i) {
while (smooth_model.isInitializing())
    {
        smooth_model.initialize(training_data[i]);
        i++;
    }
}

// Mean Square Error (MSE)
float calculate_mse(const std::vector<float> &actual, const std::vector<float> &predicted)
{
    float error_sum = 0.0;
    for (size_t i = 0; i < actual.size(); ++i)
    {
        error_sum += (actual[i] - predicted[i]) * (actual[i] - predicted[i]);
    }
    return error_sum / actual.size();
}

void get_predictions(HoltLinearModel &model, const std::vector<float> &training_data, std::vector<float> &predictions, float &max_asc, float &max_des, size_t i)
{
    for (; i < training_data.size(); ++i) {
        float prediction = model.forecast();
        predictions.push_back(prediction);
        model.update(training_data[i]);
        max_asc = std::max(max_asc, model.get_trend());
        max_des = std::min(max_des, model.get_trend());
    }
}

// Function to perform grid search for the best alpha and beta values
void grid_search(const std::vector<float> &training_data, float &best_alpha, float &best_beta, float &best_max_asc, float &best_max_des) {
    std::vector<float> predictions;
    float best_mse = std::numeric_limits<float>::max();
    for (float alpha = 0.1; alpha <= 0.9; alpha += 0.05) {
        for (float beta = 0.1; beta <= 0.9; beta += 0.05) {
            HoltLinearModel model(alpha, beta);
            size_t i = 0;
            initialize_HoltLinearModel(model, training_data, i);

            predictions.clear();
            float max_asc = 0;
            float max_des = 0;
            get_predictions(model, training_data, predictions, max_asc, max_des, i);

            float mse = calculate_mse(training_data, predictions);
            if (mse < best_mse) {
                best_mse = mse;
                best_alpha = alpha;
                best_beta = beta;
                best_max_asc = max_asc;
                best_max_des = max_des;
            }
        }
    }
}

SmoothingParameters tune_smooth_param(std::vector<float> &training_data, float tolerance)
{
    float best_mse = std::numeric_limits<float>::max();
    float best_alpha, best_beta, best_max_asc, best_max_des;
    std::vector<float> predictions;

    grid_search(training_data, best_alpha, best_beta, best_max_asc, best_max_des);

    best_max_asc += (tolerance / 2) * (best_max_asc + (-best_max_des));
    best_max_des -= (tolerance / 2) * (best_max_asc + (-best_max_des));

    return SmoothingParameters({best_alpha, best_beta, best_max_asc, best_max_des});
}

void initialize_WindowModel(SlidingWindow& window_model, HoltLinearModel& smooth_model, const std::vector<float>& training_data, size_t& i) {
    while (window_model.isInitializing()) {
        float prediction = smooth_model.forecast();
        window_model.update(training_data[i] - prediction);
        smooth_model.update(training_data[i]);
        i++;
    }
}

void measure_window_params(SlidingWindow& window_model, HoltLinearModel& smooth_model, const std::vector<float>& training_data, size_t& i, float& min_sd, float& max_sd, float& max_sd_from_me, float& min_st_from_me) {
    for (; i < training_data.size(); i++) {
        float prediction = smooth_model.forecast();
        float real_value = training_data[i];
        float difference = real_value - prediction;
        window_model.update(difference);
        float sd = window_model.get_sd();

        max_sd = std::max(max_sd, sd);
        min_sd = std::min(min_sd, sd);

        float me = window_model.get_me();
        float sd_from_me = std::abs(me - difference) / sd;
        max_sd_from_me = std::max(max_sd_from_me, sd_from_me);
        min_st_from_me = std::min(min_st_from_me, sd_from_me);

        smooth_model.update(real_value);
    }
}

WindowParameters calculate_window_params(float max_sd, float min_sd, float max_sd_from_me, float min_st_from_me, float tolerance) {
    WindowParameters window_params;
    window_params.max_sd = max_sd + tolerance * (max_sd - min_sd) / 2;
    window_params.min_sd = std::max(float(0), min_sd - tolerance * (max_sd - min_sd) / 2);
    window_params.threshold = max_sd_from_me + tolerance * (max_sd_from_me - min_st_from_me);
    return window_params;
}

WindowParameters tune_window_param(std::vector<float> &training_data, size_t window_size, float tolerance, SmoothingParameters smooth_param)
{
    WindowParameters best_params({window_size, 0, std::numeric_limits<float>::max(), 0});
    HoltLinearModel smooth_model = HoltLinearModel(smooth_param.alpha, smooth_param.beta);
    size_t i = 0;
    initialize_HoltLinearModel(smooth_model, training_data, i);

    SlidingWindow window_model = SlidingWindow(window_size);
    initialize_WindowModel(window_model, smooth_model, training_data, i);
    
    float min_sd = std::numeric_limits<float>::max();
    float max_sd = 0;
    float max_sd_from_me = 0;
    float min_st_from_me = std::numeric_limits<float>::max();

    measure_window_params(window_model, smooth_model, training_data, i, min_sd, max_sd, max_sd_from_me, min_st_from_me);

    return calculate_window_params(max_sd, min_sd, max_sd_from_me, min_st_from_me, tolerance);
}

TunnedParameters find_optimal_param(std::vector<float> &training_data, size_t window_size, float tolerance)
{
    SmoothingParameters smooth_model = tune_smooth_param(training_data, tolerance);
    WindowParameters window_model = tune_window_param(training_data, window_size, tolerance, smooth_model);
    return {smooth_model, window_model};
}
