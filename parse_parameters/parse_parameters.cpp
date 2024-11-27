#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <sstream>
#include "parse_parameters.h"
#include "../signal_validator/signal_validator.h"

const std::string USAGE_EXPLANATION = "Usage: suvm_runner mode=tune t_file=<path/to/file.csv> l_limit=<num> u_limit=<num> [o_file=<filename> tol=<num> w_size=<num>] \n"
                                      "suvm_runner mode=validate v_file=<path/to/file.csv> c_file=<path/to/c_file.ini> [o_file=<filename> o_format=<long|short>] \n"
                                      "suvm_runner mode=tune_validate t_file=<path/to/file.csv> v_file=<path/to/file.csv> l_limit=<num> u_limit=<num> [o_file=<filename> tol=<num> w_size=<num> o_format=<long|short>] \n\n"
                                      "mode: tune, validate, tune_validate\n"
                                      "w_size: size of the sliding window\n"
                                      "tol: tolerance for the measured thresholds\n"
                                      "l_limit: lower limit of value\n"
                                      "u_limit: upper limit of value\n"
                                      "t_file: path to the csv tuning file\n"
                                      "v_file: path to the csv validation file\n"
                                      "c_file: path to the configuration file\n"
                                      "o_file: output filename without extension\n"
                                      "o_format: output format\n";

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
    if (argc < 2 || (argc == 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h"))) {
        throw std::invalid_argument(USAGE_EXPLANATION);
    }

    // Parse arguments into a key-value map
    auto args = parseArguments(argc, argv);

    // Ensure mode is provided
    ensureKeyExists(args, "mode");
    std::string mode = args["mode"];

    // Validate required arguments based on the mode
    if (mode == "tune") {
        ensureKeyExists(args, "t_file");
        ensureKeyExists(args, "u_limit");
        ensureKeyExists(args, "l_limit");
    } else if (mode == "validate") {
        ensureKeyExists(args, "v_file");
        ensureKeyExists(args, "c_file");
    } else if (mode == "tune_validate") {
        ensureKeyExists(args, "t_file");
        ensureKeyExists(args, "v_file");
        ensureKeyExists(args, "u_limit");
        ensureKeyExists(args, "l_limit");
    } else {
        throw std::invalid_argument("Invalid mode: " + mode);
    }

    // Optional arguments with defaults
    std::string o_file = args.count("o_file") ? args["o_file"] : "validation_output.csv";
    std::string tolerance = args.count("tol") ? args["tol"] : "0.6"; // Default tolerance
    std::string w_size = args.count("w_size") ? args["w_size"] : "20"; // Default window size
    
    Mode mode_enum = mode == "tune" ? TUNE : (mode == "validate" ? VALIDATE : TUNE_VALIDATE);
    if (args.count("o_format") && args["o_format"] != "long" && args["o_format"] != "short") {
        throw std::invalid_argument("Invalid value for output format: " + args["o_format"]);
    }
    if (args.count("o_format") && mode_enum == TUNE) {
        throw std::invalid_argument("Output format is not valid for tune mode");
    }
    OutputFormat o_format = args.count("o_format") ? (args["o_format"] == "long" ? LONG : SHORT) : LONG;

    // Print final configuration
    SUVMParameters loaded_params({
        mode_enum,
        std::stoi(w_size),
        std::stof(tolerance),
        std::stof(args["l_limit"]),
        std::stof(args["u_limit"]),
        args.count("t_file") ? args["t_file"] : "",
        args.count("v_file") ? args["v_file"] : "",
        args.count("c_file") ? args["c_file"] : "",
        o_file,
        o_format
    });

    parameters_validation(loaded_params);
    return loaded_params;
}
