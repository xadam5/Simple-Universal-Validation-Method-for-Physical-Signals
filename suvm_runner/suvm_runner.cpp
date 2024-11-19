// Simple Universal Validation Method (SUVM) Runner
#include <iostream>
#include <fstream>
#include <vector>
#include "../signal_validator/signal_validator.h"
#include "../validator_handler/validator_handler.h"
#include "../auto_tuning/auto_tuning.h"
#include "../data_manipulation/CSVReader.h"

int main()
{
    auto training_file = "../sample_data/sensor_data/sensor_14/output_15.csv";
    auto test_file = "../sample_data/sensor_data/sensor_14/output_8.csv";
    auto output_file = "validation_output.csv";

    std::vector<float> training_data = readCSV(training_file);
    std::vector<float> test_data = readCSV(test_file);

    size_t window_size = 10;
    float tolerance = 0.5;

    ValidatorParameters optimal_param = find_optimal_param(training_data, window_size, tolerance);
    SignalValidator validator(optimal_param);
    ValidatorHandler handler(validator, optimal_param.smooth_param.beta, window_size);
    std::ofstream validation_output(output_file);
    validation_output << "flags,failure_count" << std::endl;
    for (auto measurement : test_data)
    {
        std::tuple<int, int> output = handler.validate(measurement);
        int status = std::get<0>(output);
        int failure_count = std::get<1>(output);
        std::vector<std::string> flags;
        flags.clear();
        if (status & INVALID)
        {
            flags.push_back("INVALID");
        }
        if (status & LOST)
        {
            flags.push_back("LOST");
        }
        if (status & INITIALIZING)
        {
            flags.push_back("INITIALIZING");
        }
        if (status & OUT_OF_RANGE)
        {
            flags.push_back("OUT_OF_RANGE");
        }
        if (status & NOT_CONTINUOUS)
        {
            flags.push_back("NOT_CONTINUOUS");
        }
        if (status & STUCK)
        {
            flags.push_back("STUCK");
        }
        if (status & NOISY)
        {
            flags.push_back("NOISY");
        }
        if (status & RAPID_INCREASE)
        {
            flags.push_back("RAPID_INCREASE");
        }
        if (status & RAPID_DECREASE)
        {
            flags.push_back("RAPID_DECREASE");
        }

        if (flags.empty())
        {
            flags.push_back("VALID");
        }

        validation_output << "{";
        for (auto flag : flags)
        {
            validation_output << flag << ",";
        }
        validation_output << "}," << failure_count << std::endl;
    }
    
    return 0;
}