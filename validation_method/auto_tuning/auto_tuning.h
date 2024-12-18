#ifndef AUTO_TUNER_H
#define AUTO_TUNER_H

#include <limits>
#include "../signal_validator/signal_validator.h"

/**
 * @struct TunnedParameters
 * @brief The tuned parameters for the signal validator.
 */
struct TunnedParameters
{
    SmoothingParameters smoothing_params;
    WindowParameters window_params;
};

/**
 * @brief Finds the optimal parameters for the signal validator.
 *
 * This function takes a vector of tuning data and finds the optimal parameters
 * for the Holts Linear Trend model and the signal validator by adjusting
 * the window size and tolerance.
 *
 * @param tuning_data Vector of float values used for tuning.
 * @param window_size Size of the sliding window for validation.
 * @param tolerance Tolerance level for tuning.
 * @return TunnedParameters containing the optimal smoothing and window parameters.
 */
TunnedParameters find_optimal_params(std::vector<float> &tuning_data, size_t wintdo_size, float tolerance);

#endif // AUTO_TUNER_H
