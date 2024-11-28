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
#include "csv_reader/csv_reader.h"
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

ValidatorParameters get_validator_params(SUVMParameters &params)
{
    std::vector<float> tuning_data;
    ValidatorParameters validator_params;
    switch (params.mode) {
        case VALIDATE:
            validator_params = load_config(params.config_file);
            break;

        default:
            tuning_data = readCSV(params.tuning_file);
            TunnedParameters optimal_param = find_optimal_param(tuning_data, params.window_size, params.tolerance);
            validator_params = {optimal_param.smooth_param,
                                optimal_param.window_param,
                                params.lower_limit,
                                params.upper_limit};
            break;
        }

        if (params.mode == TUNE) {
            generate_config(validator_params, params.config_file);
        }

        if (params.mode == TUNE_VALIDATE) {
            print_params(validator_params);
        }

        return validator_params;
}

std::vector<std::string> get_flags(int status) {
    std::vector<std::string> flags;
    if (status & INVALID) { flags.push_back("INVALID"); }
    if (status & LOST) { flags.push_back("LOST"); }
    if (status & INITIALIZING) { flags.push_back("INITIALIZING"); }
    if (status & OUT_OF_RANGE) { flags.push_back("OUT_OF_RANGE"); }
    if (status & NOT_CONTINUOUS) { flags.push_back("NOT_CONTINUOUS"); }
    if (status & STUCK) { flags.push_back("STUCK"); }
    if (status & NOISY) { flags.push_back("NOISY"); }
    if (status & RAPID_INCREASE) { flags.push_back("RAPID_INCREASE"); }
    if (status & RAPID_DECREASE) { flags.push_back("RAPID_DECREASE"); }
    if (flags.empty()) { flags.push_back("VALID"); }
    return flags;
}

void write_validation_line(std::ofstream& output, int index, float measurement, float forecast, int status, int failure_count, OutputFormat format) {
    if (format == SHORT) {
        output << index << "," <<  status << "," << failure_count << std::endl;
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

void simulate_validation(std::string output_file, std::vector<float> &test_data, SignalValidator &validator, ValidatorHandler &handler, OutputFormat output_format) {
    std::ofstream validation_output(output_file + ".csv");
    if (!validation_output.is_open()) {
        throw std::fstream::failure("Error: Could not open the file " + output_file + ".csv");
    }

    if (output_format == SHORT) {
        validation_output << "timestamp,status,failure_count" << std::endl;
    } else {
        validation_output << "timestamp,value,forecast,flags,failure_count" << std::endl;
    }
    int i = 0;
    for (auto measurement : test_data)
    {
        float forecast = validator.forecast();
        std::tuple<int, int> output = handler.validate(measurement);
        int status = std::get<0>(output);
        int failure_count = std::get<1>(output);
        write_validation_line(validation_output, i, measurement, forecast, status, failure_count, output_format);
        i++;
    }
    
    validation_output.close();
}

int main(int argc, char *argv[])
{
    try {
        SUVMParameters program_params = parse_args(argc, argv);
        ValidatorParameters validator_params = get_validator_params(program_params);
        if(program_params.mode == TUNE) {
            return 0;
        }

        SignalValidator validator(validator_params);
        ValidatorHandler handler(validator, validator_params.smooth_param.beta, validator_params.window_param.window_size);

        std::vector<float> test_data = readCSV(program_params.validation_file);
        simulate_validation(program_params.output_file, test_data, validator, handler, program_params.output_format);

    } catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}