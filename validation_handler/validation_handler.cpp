#include "validation_handler.h"

ValidationHandler::ValidationHandler(SignalValidator &validator, float trend_coeff, int window_size)
    : validator(validator),
      not_cont_threshold(10 - (5 - (1 + trend_coeff) * 10)),
      variability_threshold(window_size / 2),
      rapid_change_threshold(10 - (5 - trend_coeff * 10))
{}

std::tuple<int, int> ValidationHandler::validate(float value)
{
    int status = validator.validate(value);

    if (status & INITIALIZING)
    {
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

    update_failure_counter(status != VALID);
    return std::make_tuple(output, failure_count);
}

void ValidationHandler::range_check(int status)
{
    if (status & OUT_OF_RANGE && !(status & NOT_CONTINUOUS))
    {
        is_lost = true;
    }
}

void ValidationHandler::not_cont_check(int status)
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

void ValidationHandler::stuck_check(int status)
{
    status & STUCK ? stuck_count++ : stuck_count = 0;

    if (stuck_count > variability_threshold)
    {
        is_lost = true;
    }
}

void ValidationHandler::noisy_check(int status)
{
    status & NOISY ? noisy_count++ : noisy_count = 0;

    if (noisy_count > variability_threshold)
    {
        is_lost = true;
    }
}

void ValidationHandler::rapid_change_check(int status)
{
    status & RAPID_INCREASE || status & RAPID_DECREASE
        ? rapid_change_count++ : rapid_change_count = 0;

    if (rapid_change_count > rapid_change_threshold)
    {
        is_lost = true;
    }
}

void ValidationHandler::update_failure_counter(bool is_failure)
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
