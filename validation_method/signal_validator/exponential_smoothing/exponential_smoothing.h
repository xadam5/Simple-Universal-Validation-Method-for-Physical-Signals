#ifndef EXPONENTIAL_SMOOTHING_H
#define EXPONENTIAL_SMOOTHING_H

#include <vector>

/**
 * @class HoltLinearModel
 * @brief Implements the Holt Linear Exponential Smoothing model.
 *
 * This class provides methods to initialize, update, and forecast values
 * using the Holt Linear Exponential Smoothing technique.
 */
class HoltLinearModel
{
public:
    /**
     * @brief Constructor for HoltLinearModel.
     * @param alpha Smoothing factor for the level.
     * @param beta Smoothing factor for the trend.
     */
    HoltLinearModel(float alpha, float beta);

    /**
     * @brief Initializes the model with the value.
     * @param value Initial value to start the model.
     */
    void initialize(float value);

    /**
     * @brief Updates the model with a new value.
     * @param new_value New value to update the model.
     */
    void update(float new_value);

    /**
     * @brief Forecasts the next value based on the current model state.
     * @return Forecasted value.
     */
    float forecast();

    /**
     * @brief Checks if the model is still in the initialization phase.
     * @return True if the model is initializing, false otherwise.
     */
    bool is_initializing();

    /**
     * @brief Gets the current trend value.
     * @return Current trend value.
     */
    float get_trend();

private:
    float alpha;       ///< Smoothing factor for the level.
    float beta;        ///< Smoothing factor for the trend.
    float level;       ///< Current level value.
    float trend;       ///< Current trend value.
    bool initialized;  ///< Flag indicating if the model is initialized.
    int init_len;      ///< Length of the initialization phase.
    int init_count;    ///< Counter for the initialization phase.
};

#endif // EXPONENTIAL_SMOOTHING_H