#include <iostream>
#include <deque>
#include <cmath>
#include <vector>
#include <tuple>
#include "sliding_window.h"

SlidingWindow::SlidingWindow(size_t window_size, float threshold)
    : window_size(window_size), threshold(threshold), window() {}

float SlidingWindow::mean()
{
    float window_sum = 0;
    for (float val : window)
    {
        window_sum += val;
    }
    return window_sum / window.size();
}

float SlidingWindow::standard_deviation()
{
    float me = mean();
    float var = 0;
    for (float val : window)
    {
        var += (val - me) * (val - me);
    }
    return std::sqrt(var / window.size());
}

void SlidingWindow::update(float value)
{
    if (window.size() == window_size)
    {
        window.pop_front();
    }
    window.push_back(value);
}

bool SlidingWindow::isOutlier(float value)
{
    if (window.size() < window_size)
    {
        return true;
    }

    float me = mean();
    float sd = standard_deviation();
    return std::abs(value) <= me + threshold * sd;
}

bool SlidingWindow::isInitializing()
{
    return window.size() != window_size;
}
