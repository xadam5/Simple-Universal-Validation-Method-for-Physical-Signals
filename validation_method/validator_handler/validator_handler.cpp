#include "validator_handler.h"
#include <tuple>
#include <cmath>

ValidatorHandler::ValidatorHandler(SignalValidator &validator, float trend_coeff, int window_size, int invalid_threshold)
    : validator(validator),
      missing_value_threshold(invalid_threshold),
      not_cont_threshold(invalid_threshold + (invalid_threshold / 2 - (1 - trend_coeff) * invalid_threshold)),
      variability_threshold(window_size / 2),
      rapid_change_threshold(invalid_threshold - (invalid_threshold / 2 - trend_coeff * invalid_threshold))
{}

std::tuple<int, int> ValidatorHandler::validate(float value)
{
    if (std::isnan(value))
    {
        missing_value_count++;
        update_failure_counter(true);
        validator.update(validator.forecast());
        int status = INVALID | MISSING_VALUE;
        if (missing_value_count > missing_value_threshold)
        {
            is_lost = true;
            status |= LOST;
        }
        return std::make_tuple(status, failure_count);
    }
    else
    {
        missing_value_count = 0;
    }

    int status = validator.validate(value);

    if (status & INITIALIZING)
    {
        validator.update(value);
        return std::make_tuple(INITIALIZING, 0);
    }

    range_check(status);
    not_cont_check(status);
    stuck_check(status);
    noisy_check(status);
    rapid_change_check(status);

    int output = status;
    if (status != VALID)
    {
        output |= INVALID;
    }

    if (is_lost)
    {
        output |= LOST;
    }

    if (status & NOT_CONTINUOUS)
    {
        validator.update(validator.forecast());
    }
    else
    {
        validator.update(value);
    }

    update_failure_counter(status != VALID);
    return std::make_tuple(output, failure_count);
}

void ValidatorHandler::missing_check(int status)
{
    status & MISSING_VALUE ? missing_value_count++ : missing_value_count = 0;

    if (missing_value_count > missing_value_threshold)
    {
        is_lost = true;
    }
}

void ValidatorHandler::range_check(int status)
{
    if (status & OUT_OF_RANGE && !(status & NOT_CONTINUOUS))
    {
        is_lost = true;
    }
}

void ValidatorHandler::not_cont_check(int status)
{
    if ((status & NOT_CONTINUOUS && (status & RAPID_DECREASE || status & NOISY)))
    {
        is_lost = true;
    }

    status & NOT_CONTINUOUS ? not_cont_count++ : not_cont_count = 0;

    if (not_cont_count > not_cont_threshold)
    {
        is_lost = true;
    }
}

void ValidatorHandler::stuck_check(int status)
{
    status & STUCK ? stuck_count++ : stuck_count = 0;

    if (stuck_count > variability_threshold)
    {
        is_lost = true;
    }
}

void ValidatorHandler::noisy_check(int status)
{
    status & NOISY ? noisy_count++ : noisy_count = 0;

    if (noisy_count > variability_threshold)
    {
        is_lost = true;
    }
}

void ValidatorHandler::rapid_change_check(int status)
{
    status & RAPID_INCREASE || status & RAPID_DECREASE
        ? rapid_change_count++ : rapid_change_count = 0;

    if (rapid_change_count > rapid_change_threshold)
    {
        is_lost = true;
    }
}

void ValidatorHandler::update_failure_counter(bool is_failure)
{
    if (failures[99])
    {
        failure_count--;
    }
    
    failures <<= 1;
    if (is_failure)
    {
        failures.set(0);
        failure_count++;
    }
}
