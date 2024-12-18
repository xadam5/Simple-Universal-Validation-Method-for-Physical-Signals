#include <bitset>
#include "../signal_validator/signal_validator.h"

/**
 * @enum OutputStatus
 * @brief Defines the output status codes for validation.
 */
enum OutputStatus
{
    INVALID = 0b1000000000, ///< Indicates an invalid status.
    LOST = 0b100000000,     ///< Indicates a lost status.
};

/**
 * @class ValidatorHandler
 * @brief Handles the validation of signals using various checks.
 *
 * This class provides manipulation with the validator and handling the validator
 * status. It maintains counters for different types of failure and declares
 * validator as lost, when certain counter reachest threshold.
 */
class ValidatorHandler
{
public:
    /**
     * @brief Constructor for ValidatorHandler.
     * @param validator Reference to a SignalValidator object.
     * @param trend_coeff Coefficient for trend validation.
     * @param window_size Size of the sliding window for validation.
     * @param invalid_threshold Threshold for invalid values.
     */
    ValidatorHandler(SignalValidator &validator, float trend_coeff, int window_size, int invalid_threshold);

    /**
     * @brief Validates a given value.
     * @param value Value to be validated.
     * @return A tuple containing the validation status and the failure count.
     */
    std::tuple<int, int> validate(float value);

private:
    SignalValidator &validator; ///< Reference to the SignalValidator object.
    std::bitset<100> failures;  ///< Reference to the SignalValidator object.
    int failure_count = 0;      ///< Counter for the number of failures.

    /**
     * @brief Updates the failure counter based on the failure status.
     * @param is_failure Boolean indicating if the current value is a failure.
     */
    void update_failure_counter(bool is_failure);

    bool is_lost = false;                ///< Flag indicating if the signal is lost.
    int missing_value_count = 0;        ///< Counter for missing values.
    const int missing_value_threshold;  ///< Threshold for missing values.

    /**
     * @brief Checks for missing values based on the status.
     * @param status Current status of the signal.
     */
    void missing_check(int status);

    /**
     * @brief Checks whether the signal is in the valid range based on the status.
     * @param status Current status of the signal.
     */
    void range_check(int status);

    int not_cont_count = 0;         ///< Counter for non-continuous values.
    const int not_cont_threshold;   ///< Threshold for non-continuous values.

    /**
     * @brief Checks for non-continuous values based on the status.
     * @param status Current status of the signal.
     */
    void not_cont_check(int status);

    int stuck_count = 0;              ///< Counter for stuck values.
    int noisy_count = 0;              ///< Counter for noisy values.
    const int variability_threshold;  ///< Threshold for variability.

    /**
     * @brief Checks for stuck values based on the status.
     * @param status Current status of the signal.
     */
    void stuck_check(int status);

    /**
     * @brief Checks for noisy values based on the status.
     * @param status Current status of the signal.
     */
    void noisy_check(int status);

    int rapid_change_count = 0;         ///< Counter for rapid changes.
    const int rapid_change_threshold;   ///< Threshold for rapid changes.

    /**
     * @brief Checks for rapid changes based on the status.
     * @param status Current status of the signal.
     */
    void rapid_change_check(int status);
};