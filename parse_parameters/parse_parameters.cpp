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
        ensureKeyExists(args, "training_file");
        ensureKeyExists(args, "upper_limit");
        ensureKeyExists(args, "lower_limit");
    } else if (mode == "validate") {
        ensureKeyExists(args, "validation_file");
        ensureKeyExists(args, "config_file");
    } else if (mode == "tune_validate") {
        ensureKeyExists(args, "training_file");
        ensureKeyExists(args, "test_file");
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
    OutputFormat output_format = args.count("output_format") ? (args["output_format"] == "long" ? LONG : SHORT) : LONG;

    // Print final configuration
    SUVMParameters loaded_params(
        mode_enum,
        std::stoi(window_size),
        std::stof(tolerance),
        std::stof(args["upper_limit"]),
        std::stof(args["lower_limit"]),
        args.count("training_file") ? args["training_file"] : "",
        args.count("test_file") ? args["test_file"] : "",
        args.count("validation_file") ? args["validation_file"] : "",
        output_file,
        output_format
    );

    return loaded_params;
}
