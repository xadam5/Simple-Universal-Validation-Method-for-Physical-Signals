#ifndef SLIDING_WINDOW_H
#define SLIDING_WINDOW_H

#include <deque>
#include <vector>
#include <tuple>

class SlidingWindow {
public:
    SlidingWindow(const size_t window_size, const float threshold);
    float mean();
    float standard_deviation();
    void update(float value);
    bool validate(float value);
    bool isInitializing();

private:
    const size_t window_size;
    const float threshold;
    std::deque<float> window;
};

#endif
