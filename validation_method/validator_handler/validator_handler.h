#include <bitset>
#include "../signal_validator/signal_validator.h"

enum OutputStatus
{
    INVALID = 0b1000000000,
    LOST = 0b100000000,
};

class ValidatorHandler
{
public:
    ValidatorHandler(SignalValidator &validator, float trend_coeff, int window_size, int invalid_threshold);
    std::tuple<int, int> validate(float value);

private:
    SignalValidator &validator;
    std::bitset<100> failures;
    int failure_count = 0;
    void update_failure_counter(bool is_failure);

    bool is_lost = false;
    int missing_value_count = 0;
    const int missing_value_threshold;
    void missing_check(int status);
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