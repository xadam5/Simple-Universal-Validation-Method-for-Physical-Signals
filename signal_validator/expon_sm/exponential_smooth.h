#ifndef EXPONENTIAL_SMOOTH_H
#define EXPONENTIAL_SMOOTH_H

#include <vector>

class HoltLinearModel
{
public:
    HoltLinearModel(float alpha, float beta);
    void initialize(float value);
    void update(float new_value);
    float forecast();
    bool isInitializing();
    float get_trend();

private:
    float alpha;
    float beta;
    float level;
    float trend;
    bool initialized;
    int init_len;
    int init_count;
};

#endif
