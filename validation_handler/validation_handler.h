#include <bitset>
#include "../signal_validator/signal_validator.h"

enum OutputStatusMask
{
    VALID           = 0b0,
    INVALID         = 0b100000000,
    LOST            = 0b10000000,
    INITIALIZING    = 0b1000000,
    OUT_OF_RANGE    = 0b100000,
    NOT_CONTINUOUS  = 0b10000,
    STUCK           = 0b1000,
    NOISY           = 0b100,
    RAPID_INCREASE  = 0b10,
    RAPID_DECREASE  = 0b1,

};

class ValidationHandler
{
public:
    ValidationHandler(SignalValidator &validator, float trend_coeff, int window_size);
    std::tuple<int, int> validate(float value);

private:
    SignalValidator &validator;
    std::bitset<100> failures;
    int failure_count = 0;
    void update_failure_counter(bool is_failure);

    bool is_lost = false;
    void range_check(int status);

    int not_cont_count = 0;
    const int not_cont_threshold;
    void not_cont_check(int status);

    int stuck_count = 0;
    int noisy_count = 0;
    const int variability_threshold;
    void stuck_check(int status);
    void noisy_check(int status);

    int rapid_change_count = 0;
    const int rapid_change_threshold;
    void rapid_change_check(int status);
};