#ifndef SLIDING_WINDOW_H
#define SLIDING_WINDOW_H

#include <deque>
#include <vector>

/**
 * @class SlidingWindow
 * @brief Implements a sliding window for calculating mean and standard deviation.
 *
 * This class provides methods to update the window with new values, and to
 * calculate the mean and standard deviation of the values within the window.
 */
class SlidingWindow
{
public:
    /**
     * @brief Constructor for SlidingWindow.
     * @param window_size Size of the sliding window.
     */
    SlidingWindow(size_t window_size);

    /**
     * @brief Gets the mean of the values in the window.
     * @return Mean of the values.
     */
    float get_me();

    /**
     * @brief Gets the standard deviation of the values in the window.
     * @return Standard deviation of the values.
     */
    float get_sd();

    /**
     * @brief Updates the window with a new value.
     * @param value New value to add to the window.
     */
    void update(float value);

    /**
     * @brief Checks if the window is still in the initialization phase.
     * @return True if the window is initializing, false otherwise.
     */
    bool is_initializing();

private:
    size_t window_size;       ///< Size of the sliding window.
    std::deque<float> window; ///< Deque to store the values in the window.
    float me;                 ///< Mean of the values in the window.
    float sd;                 ///< Standard deviation of the values in the window.
    
    /**
     * @brief Updates the mean of the values in the window.
     */
    void update_me();

    /**
     * @brief Updates the standard deviation of the values in the window.
     */
    void update_sd();
};

#endif // SLIDING_WINDOW_H
