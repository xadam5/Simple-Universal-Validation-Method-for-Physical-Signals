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
    auto training_file = "../sample_data/noise_change/normal_simulated_data.csv";
    auto test_file = "../sample_data/noise_change/noise_increase_data.csv";
    auto output_file = "simulations/validation_output_noise_change.csv";

    std::vector<float> training_data = readCSV(training_file);
    std::vector<float> test_data = readCSV(test_file);

    size_t window_size = 20;
    float tolerance = 0.6;

    ValidatorParameters optimal_param = find_optimal_param(training_data, window_size, tolerance);
    optimal_param.range_l_limit = -10;
    optimal_param.range_u_limit = 20;
    std::cout << "Optimal Parameters: " << std::endl << optimal_param.smooth_param.alpha << " Alpha; " << optimal_param.smooth_param.beta << " Beta; "  << optimal_param.window_param.threshold << " cont. threshold;" << std::endl;
    std::cout << optimal_param.window_param.window_size << " Window Size; " << optimal_param.window_param.min_sd << " Min SD; " << optimal_param.window_param.max_sd << " Max SD; " << std::endl;
    std::cout << optimal_param.range_l_limit << " Lower Limit; " << optimal_param.range_u_limit << " Upper Limit; " << std::endl;
    std::cout << optimal_param.smooth_param.max_asc << " Max Asc; " << optimal_param.smooth_param.max_des << " Max Des; " << std::endl;
    SignalValidator validator(optimal_param);
    ValidatorHandler handler(validator, optimal_param.smooth_param.beta, window_size);
    std::ofstream validation_output(output_file);
    validation_output << ",value,forecast,flags,failure_count" << std::endl;
    int i = 0;
    for (auto measurement : test_data)
    {
        float forecast = validator.forecast();
        validation_output << i << "," << measurement << "," << forecast << ",";
        i++;
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

        validation_output << "\"";
        int i = 0;
        for (auto flag : flags)
        {
            if (i == 0)
            {
                validation_output << flag;
                i++;
            }
            else
            validation_output << "|" <<  flag;
        }
        validation_output << "\"," << failure_count << std::endl;
    }
    
    return 0;
}