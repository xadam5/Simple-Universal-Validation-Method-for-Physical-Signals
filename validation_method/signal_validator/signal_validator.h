#ifndef SIGNAL_VALIDATOR_H
#define SIGNAL_VALIDATOR_H

#include <vector>
#include "exponential_smoothing/exponential_smoothing.h"
#include "sliding_window/sliding_window.h"

/**
 * @struct SmoothingParameters
 * @brief Parameters for exponential smoothing.
 */
struct SmoothingParameters
{
    float alpha;    ///< Smoothing factor for the level.
    float beta;     ///< Smoothing factor for the trend.
    float max_asc;  ///< Maximum allowable ascent rate.
    float max_des;  ///< Maximum allowable descent rate.
};

/**
 * @struct WindowParameters
 * @brief Parameters for the sliding window.
 */
struct WindowParameters
{
    size_t window_size;  ///< Size of the sliding window.
    float min_sd;        ///< Minimum standard deviation.
    float max_sd;        ///< Maximum standard deviation.
    float max_z_score;   ///< Maximum z-score.
};

/**
 * @struct ValidatorParameters
 * @brief All parameters for the signal validator.
 */
struct ValidatorParameters
{
    SmoothingParameters smoothing_params; ///< Parameters for exponential smoothing.
    WindowParameters window_params;       ///< Parameters for the sliding window.
    float range_l_limit;                  ///< Lower limit for the valid range.
    float range_u_limit;                  ///< Upper limit for the valid range.
    int invalid_threshold;                ///< Threshold for invalid values.
};

/**
 * @enum ValidationStatus
 * @brief Defines the validation status codes.
 */
enum ValidationStatus
{
    VALID           = 0b0,          ///< Indicates a valid status.
    INITIALIZING    = 0b10000000,   ///< Indicates the model is initializing.
    MISSING_VALUE   = 0b1000000,    ///< Indicates a missing value.
    OUT_OF_RANGE    = 0b100000,     ///< Indicates the value is out of range.
    NOT_CONTINUOUS  = 0b10000,      ///< Indicates the value is not continuous.
    STUCK           = 0b1000,       ///< Indicates the value is stuck.
    NOISY           = 0b100,        ///< Indicates the value is noisy.
    RAPID_INCREASE  = 0b10,         ///< Indicates a rapid increase.
    RAPID_DECREASE  = 0b1,          ///< Indicates a rapid decrease.
};

/**
 * @class SignalValidator
 * @brief Validates signals using exponential smoothing and sliding window techniques.
 *
 * This class provides methods to update the signal with new values, forecast future values,
 * and validate the signal based on various criteria such as range, continuity, noise, and rapid changes.
 */
class SignalValidator
{
public:
    /**
     * @brief Constructor for SignalValidator.
     * @param params Parameters for the signal validator.
     */
    SignalValidator(ValidatorParameters params);

    /**
     * @brief Updates the signal with a new value.
     * @param new_value New value to update the signal.
     */
    void update(float new_value);

    /**
     * @brief Forecasts the next value based on the current model state.
     * @return Forecasted value.
     */
    float forecast();

    /**
     * @brief Validates a given value.
     * @param value Value to be validated.
     * @return Validation status code.
     */
    int validate(float value);

private:
    HoltLinearModel smoothing_model;    ///< Holt Linear Trend model.
    SlidingWindow window_model;         ///< Sliding window model.
    bool is_initializing = true;        ///< Flag indicating if the model is initialized.

    /**
     * @brief Initializes the model with the value.
     * @param value Value for initializing the model.
     */
    void initialize(float value);

    float range_l_limit;        ///< Lower limit for the valid range.
    float range_u_limit;        ///< Upper limit for the valid range.
    float max_z_score;          ///< Maximum z-score for continuity.
    float min_sd;               ///< Minimum standard deviation.
    float max_sd;               ///< Maximum standard deviation.
    float max_asc;              ///< Maximum ascent rate.
    float max_des;              ///< Maximum descent rate.

    /**
     * @brief Checks if the value is NOT within the valid range.
     * @param value Value to be checked.
     * @return True if the value is NOT within range, false otherwise.
     */
    bool range_check(float value);

    /**
     * @brief Checks if the value is NOT continuous.
     * @param value Value to be checked.
     * @return True if the value is NOT continuous, false otherwise.
     */
    bool continuity_check(float value);

    /**
     * @brief Checks if the values in Sliding Window model are stuck.
     * @return True if the values in Sliding Window model are stuck, false otherwise.
     */
    bool stuck_check();

    /**
     * @brief Checks if the values in Sliding Window model are noisy.
     * @return True if the values in Sliding Window model are noisy, false otherwise.
     */
    bool noise_check();

    /**
     * @brief Checks if there is a rapid increase.
     * @return True if there is a rapid increase, false otherwise.
     */
    bool increase_check();

    /**
     * @brief Checks if there is a rapid decrease.
     * @return True if there is a rapid decrease, false otherwise.
     */
    bool decrease_check();
};

#endif // SIGNAL_VALIDATOR_H
