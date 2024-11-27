#ifndef SLIDING_WINDOW_H
#define SLIDING_WINDOW_H

#include <deque>
#include <vector>

class SlidingWindow
{
public:
    SlidingWindow(size_t window_size);
    float get_me();
    float get_sd();
    void update(float value);
    bool isInitializing();

private:
    size_t window_size;
    std::deque<float> window;
    float me, sd;
    void update_me();
    void update_sd();
};

#endif
