// Simple Universal Validation Method (SUVM) Runner
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <stdexcept>
#include <limits>
#include "parse_parameters/parse_parameters.h"
#include "../signal_validator/signal_validator.h"
#include "../validator_handler/validator_handler.h"
#include "../auto_tuning/auto_tuning.h"
#include "../data_manipulation/CSVReader.h"
#include "load_config/config_handler.h"

void print_params(ValidatorParameters params) {
    std::cout << "Alpha: " << params.smooth_param.alpha << " "
              << "Beta: " << params.smooth_param.beta << std::endl;
    std::cout << "Continuity threshold: " << params.window_param.threshold << std::endl;
    std::cout << "Window Size: " << params.window_param.window_size << std::endl;
    std::cout << "Min SD: " << params.window_param.min_sd << " "
              << "Max SD: " << params.window_param.max_sd << std::endl;
    std::cout << "Lower Limit: " << params.range_l_limit << " "
              << "Upper Limit: " << params.range_u_limit << std::endl;
    std::cout << "Max Asc: " << params.smooth_param.max_asc << " "
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

void write_validation_output(std::ofstream& output, int index, float measurement, float forecast, int status, int failure_count, OutputFormat format) {
    if (format == SHORT) {
        output << index << "," << measurement << "," << status << "," << failure_count << std::endl;
        return;
    }

    output << index << "," << measurement << "," << forecast << ",\"";
    std::vector<std::string> flags = get_flags(status);
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

    ValidatorParameters validator_params;
    if (params.mode == TUNE) {
        std::vector<float> tuning_data = readCSV(params.tuning_file);
        TunnedParameters optimal_param = find_optimal_param(tuning_data, params.window_size, params.tolerance);
        validator_params = {optimal_param.smooth_param,
                            optimal_param.window_param,
                            params.lower_limit,
                            params.upper_limit};

        generate_config(validator_params, params.output_file);
        return 0;
    }

    if (params.mode == VALIDATE) {
        try
        {
            validator_params = load_config(params.config_file);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 1;
        }
        print_params(validator_params);
    }

    if (params.mode == TUNE_VALIDATE) {
        std::vector<float> tuning_data = readCSV(params.tuning_file);
        TunnedParameters optimal_param = find_optimal_param(tuning_data, params.window_size, params.tolerance);
        validator_params = {optimal_param.smooth_param,
                            optimal_param.window_param,
                            params.lower_limit,
                            params.upper_limit};
        
        print_params(validator_params);
    }

    SignalValidator validator(validator_params);
    ValidatorHandler handler(validator, validator_params.smooth_param.beta, validator_params.window_param.window_size);

    std::vector<float> test_data = readCSV(params.validation_file);
    std::ofstream validation_output(params.output_file + ".csv");
    validation_output << ",value,forecast,flags,failure_count" << std::endl;
    int i = 0;
    for (auto measurement : test_data)
    {
        float forecast = validator.forecast();
        std::tuple<int, int> output = handler.validate(measurement);
        int status = std::get<0>(output);
        int failure_count = std::get<1>(output);
        write_validation_output(validation_output, i, measurement, forecast, status, failure_count, params.output_format);
        i++;
    }
    
    validation_output.close();
    return 0;
}