// Simple Universal Validation Method (SUVM) Runner
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <stdexcept>
#include <limits>
#include "../parse_parameters/parse_parameters.h"
#include "../signal_validator/signal_validator.h"
#include "../validator_handler/validator_handler.h"
#include "../auto_tuning/auto_tuning.h"
#include "../data_manipulation/CSVReader.h"
#include "../inih/cpp/INIReader.h"

void print_params(ValidatorParameters params) {
    std::cout << "Alpha: " << params.smooth_param.alpha 
              << "Beta: " << params.smooth_param.beta << std::endl;
    std::cout << "Continuity threshold: " << params.window_param.threshold << std::endl;
    std::cout << "Window Size: " << params.window_param.window_size << std::endl;
    std::cout << "Min SD: " << params.window_param.min_sd
              << "Max SD: " << params.window_param.max_sd << std::endl;
    std::cout << "Lower Limit: " << params.range_l_limit
              << "Upper Limit: " << params.range_u_limit << std::endl;
    std::cout << "Max Asc: " << params.smooth_param.max_asc 
              << "Max Des: " << params.smooth_param.max_des << std::endl;
}

std::vector<std::string> get_flags(int status) {
    std::vector<std::string> flags;
    if (status & INVALID) {
        flags.push_back("INVALID");
    }
    if (status & LOST) {
        flags.push_back("LOST");
    }
    if (status & INITIALIZING) {
        flags.push_back("INITIALIZING");
    }
    if (status & OUT_OF_RANGE) {
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
    return flags;
}

void write_validation_output(std::ofstream& output, int index, float measurement, float forecast, const std::vector<std::string>& flags, int failure_count) {
    output << index << "," << measurement << "," << forecast << ",\"";
    for (size_t i = 0; i < flags.size(); ++i) {
        output << flags[i];
        if (i < flags.size() - 1) {
            output << "|";
        }
    }
    output << "\"," << failure_count << std::endl;
}

int main(int argc, char *argv[])
{
    SUVMParameters params;
    try {
        params = parse_args(argc, argv);
    } catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    INIReader reader(params.config_file);

    auto training_file = "../sample_data/noise_change/normal_simulated_data.csv";
    auto test_file = "../sample_data/noise_change/noise_increase_data.csv";
    auto output_file = "simulations/validation_output_noise_change_2.csv";

    std::vector<float> training_data = readCSV(training_file);
    std::vector<float> test_data = readCSV(test_file);

    size_t window_size = 20;
    float tolerance = 0.6;

    ValidatorParameters optimal_param = find_optimal_param(training_data, window_size, tolerance);
    optimal_param.range_l_limit = -10;
    optimal_param.range_u_limit = 20;
    print_params(optimal_param);

    SignalValidator validator(optimal_param);
    ValidatorHandler handler(validator, optimal_param.smooth_param.beta, window_size);
    std::ofstream validation_output(output_file);
    validation_output << ",value,forecast,flags,failure_count" << std::endl;
    int i = 0;
    for (auto measurement : test_data)
    {
        float forecast = validator.forecast();
        std::tuple<int, int> output = handler.validate(measurement);
        int status = std::get<0>(output);
        int failure_count = std::get<1>(output);
        std::vector<std::string> flags = get_flags(status);
        write_validation_output(validation_output, i, measurement, forecast, flags, failure_count);
        i++;
    }
    
    validation_output.close();
    return 0;
}