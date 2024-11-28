#include <iostream>
#include <deque>
#include <cmath>
#include <vector>
#include "sliding_window.h"

SlidingWindow::SlidingWindow(size_t window_size)
    : window_size(window_size), window(), me(0), sd(0) {}

float SlidingWindow::get_me()
{
    return me;
}

float SlidingWindow::get_sd()
{
    return sd;
}

void SlidingWindow::update_me()
{
    float window_sum = 0;
    for (float val : window)
    {
        window_sum += val;
    }
    me = window_sum / window.size();
}

void SlidingWindow::update_sd()
{
    float me = get_me();
    float var = 0;
    for (float val : window)
    {
        var += (val - me) * (val - me);
    }
    sd = std::sqrt(var / window.size());
}

void SlidingWindow::update(float value)
{
    if (window.size() == window_size)
    {
        window.pop_front();
    }
    window.push_back(value);
    update_me();
    update_sd();
}

bool SlidingWindow::isInitializing()
{
    return window.size() != window_size;
}
