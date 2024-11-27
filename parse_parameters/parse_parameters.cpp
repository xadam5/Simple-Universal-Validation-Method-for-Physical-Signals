#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <sstream>
#include "parse_parameters.h"
#include "../signal_validator/signal_validator.h"

// Helper to check if a required key exists
void ensureKeyExists(const std::map<std::string, std::string> &params, const std::string &key) {
    if (params.find(key) == params.end()) {
        throw std::invalid_argument("Missing required argument: " + key);
    }
}

// Helper to parse command-line arguments
std::map<std::string, std::string> parseArguments(int argc, char *argv[]) {
    std::map<std::string, std::string> arguments;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        auto pos = arg.find('=');
        if (pos != std::string::npos) {
            std::string key = arg.substr(0, pos);
            std::string value = arg.substr(pos + 1);
            arguments[key] = value;
        } else {
            throw std::invalid_argument("Invalid argument format: " + arg);
        }
    }
    return arguments;
}

void parameters_validation(SUVMParameters params) {
    if (params.tolerance < 0) {
        throw std::invalid_argument("Tolerance must be non-negative");
    }

    if (params.window_size <= 3) {
        throw std::invalid_argument("Window size must at least 3");
    }

    if (params.mode != VALIDATE && params.upper_limit <= params.lower_limit) {
            throw std::invalid_argument("Upper limit must be greater than lower limit");
    }
}

SUVMParameters parse_args(int argc, char *argv[]) {
    if (argc < 2) {
        throw std::invalid_argument("Usage: mode=<tune|validate|tune_validate> [key=value ...]\n");
    }

    // Parse arguments into a key-value map
    auto args = parseArguments(argc, argv);

    // Ensure mode is provided
    ensureKeyExists(args, "mode");
    std::string mode = args["mode"];

    // Validate required arguments based on the mode
    if (mode == "tune") {
        ensureKeyExists(args, "tuning_file");
        ensureKeyExists(args, "upper_limit");
        ensureKeyExists(args, "lower_limit");
    } else if (mode == "validate") {
        ensureKeyExists(args, "validation_file");
        ensureKeyExists(args, "config_file");
    } else if (mode == "tune_validate") {
        ensureKeyExists(args, "tuning_file");
        ensureKeyExists(args, "validation_file");
        ensureKeyExists(args, "upper_limit");
        ensureKeyExists(args, "lower_limit");
    } else {
        throw std::invalid_argument("Invalid mode: " + mode);
    }

    // Optional arguments with defaults
    std::string output_file = args.count("output_file") ? args["output_file"] : "validation_output.csv";
    std::string tolerance = args.count("tolerance") ? args["tolerance"] : "0.6"; // Default tolerance
    std::string window_size = args.count("window_size") ? args["window_size"] : "20"; // Default window size
    
    Mode mode_enum = mode == "tune" ? TUNE : (mode == "validate" ? VALIDATE : TUNE_VALIDATE);
    if (args.count("output_format") && args["output_format"] != "long" && args["output_format"] != "short") {
        throw std::invalid_argument("Invalid value for output format: " + args["output_format"]);
    }
    if (args.count("output_format") && mode_enum == TUNE) {
        throw std::invalid_argument("Output format is not valid for tune mode");
    }
    OutputFormat output_format = args.count("output_format") ? (args["output_format"] == "long" ? LONG : SHORT) : LONG;

    // Print final configuration
    SUVMParameters loaded_params(
        mode_enum,
        std::stoi(window_size),
        std::stof(tolerance),
        std::stof(args["upper_limit"]),
        std::stof(args["lower_limit"]),
        args.count("tuning_file") ? args["tuning_file"] : "",
        args.count("validation_file") ? args["validation_file"] : "",
        args.count("config_file") ? args["config_file"] : "",
        output_file,
        output_format
    );

    parameters_validation(loaded_params);
    return loaded_params;
}
