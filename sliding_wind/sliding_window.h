#ifndef SLIDING_WINDOW_H
#define SLIDING_WINDOW_H

#include <deque>
#include <vector>
#include <tuple>

class SlidingWindow
{
public:
    SlidingWindow(size_t window_size, float threshold);
    float mean();
    float standard_deviation();
    void update(float value);
    bool isOutlier(float value);
    bool isInitializing();

private:
    size_t window_size;
    float threshold;
    std::deque<float> window;
};

#endif
