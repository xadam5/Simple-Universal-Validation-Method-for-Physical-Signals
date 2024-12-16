#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <sstream>
#include <limits>
#include <regex>
#include "parse_parameters.h"
#include "../../validation_method/signal_validator/signal_validator.h"

const std::string USAGE_EXPLANATION = "usage: suvm_runner mode=tune t_file=<path/to/file.csv> l_limit=<num> u_limit=<num> [o_file=<filename> tol=<num> i_thr=<num> w_size=<num> ] \n"
                                      "or:    suvm_runner mode=validate v_file=<path/to/file.csv> c_file=<path/to/c_file.ini> [o_file=<filename> o_format=<long|short>] \n"
                                      "or:    suvm_runner mode=tune_validate t_file=<path/to/file.csv> v_file=<path/to/file.csv> l_limit=<num> u_limit=<num> [o_file=<filename> tol=<num> i_thr=<num> w_size=<num> o_format=<long|short> ] \n\n"
                                      "mode: tune, validate, tune_validate\n"
                                      "w_size: size of the sliding window\n"
                                      "tol: tolerance for the measured thresholds\n"
                                      "i_thr: threshold for consecutive invalid measurements\n"
                                      "l_limit: lower limit of value\n"
                                      "u_limit: upper limit of value\n"
                                      "t_file: path to the csv tuning file\n"
                                      "v_file: path to the csv validation file\n"
                                      "c_file: path to the configuration file\n"
                                      "o_file: output filename without extension\n"
                                      "o_format: output format\n";

const int DEFAULT_WINDOW_SIZE = 20;
const float DEFAULT_TOLERANCE = 1.2;
const int DEFAULT_INVALID_THRESHOLD = 10;

// Helper to parse command-line arguments
std::map<std::string, std::string> parse_arguments(int argc, char *argv[]) {
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

void ensure_key_exists(const std::map<std::string, std::string> &params, const std::string &key) {
    if (params.find(key) == params.end()) {
        throw std::invalid_argument("Missing required argument: " + key);
    }
}

bool is_int(const std::string &s) {
    return !s.empty() && std::all_of(s.begin() + 1, s.end(), ::isdigit) && (s[0] == '-' || ::isdigit(s[0]));
}

bool is_float(const std::string &s) {
    static const std::regex number_regex(
        R"(^[-]?\d+(\.\d+)?$)"
    );
    return std::regex_match(s, number_regex);
}

void validate_numeral_arguments(const std::map<std::string, std::string>& args) {
    if (args.count("w_size") && !is_int(args.at("w_size"))) {
        throw std::invalid_argument("Invalid window size: " + args.at("w_size"));
    }

    if (args.count("tol") && !is_float(args.at("tol"))) {
        throw std::invalid_argument("Invalid tolerance: " + args.at("tol"));
    }

    if (args.count("l_limit") && !is_float(args.at("l_limit"))) {
        throw std::invalid_argument("Invalid lower limit: " + args.at("l_limit"));
    }

    if (args.count("u_limit") && !is_float(args.at("u_limit"))) {
        throw std::invalid_argument("Invalid upper limit: " + args.at("u_limit"));
    }

    if (args.count("i_thr") && !is_int(args.at("i_thr"))) {
        throw std::invalid_argument("Invalid invalid threshold: " + args.at("i_thr"));
    }
}

void validate_arguments(const std::map<std::string, std::string>& args) {
    ensure_key_exists(args, "mode");
    std::string mode = args.at("mode");

    if (mode == "tune") {
        ensure_key_exists(args, "t_file");
        ensure_key_exists(args, "u_limit");
        ensure_key_exists(args, "l_limit");
        if (args.count("o_format")) {
        throw std::invalid_argument("Output format is not valid for tune mode");
        }
    } else if (mode == "validate") {
        ensure_key_exists(args, "v_file");
        ensure_key_exists(args, "c_file");
    } else if (mode == "tune_validate") {
        ensure_key_exists(args, "t_file");
        ensure_key_exists(args, "v_file");
        ensure_key_exists(args, "u_limit");
        ensure_key_exists(args, "l_limit");
    } else {
        throw std::invalid_argument("Invalid mode: " + mode);
    }

    if (args.count("o_format") && args.at("o_format") != "long" && args.at("o_format") != "short") {
        throw std::invalid_argument("Invalid value for output format: " + args.at("o_format"));
    }

    validate_numeral_arguments(args);
}

SUVMParameters arguments_to_parameters(const std::map<std::string, std::string> args) {
    SUVMParameters params;
    params.mode = args.at("mode") == "tune" ? TUNE : args.at("mode") == "validate" ? VALIDATE : TUNE_VALIDATE;
    params.window_size = args.count("w_size") ? std::stoi(args.at("w_size")) : DEFAULT_WINDOW_SIZE;
    params.tolerance = args.count("tol") ? std::stof(args.at("tol")) : DEFAULT_TOLERANCE;
    params.lower_limit = args.count("l_limit") ? std::stof(args.at("l_limit")) : std::numeric_limits<float>::quiet_NaN();
    params.upper_limit = args.count("u_limit") ? std::stof(args.at("u_limit")) : std::numeric_limits<float>::quiet_NaN();
    params.invalid_threshold = args.count("i_thr") ? std::stoi(args.at("i_thr")) : DEFAULT_INVALID_THRESHOLD;
    params.tuning_file = args.count("t_file") ? args.at("t_file") : "";
    params.validation_file = args.count("v_file") ? args.at("v_file") : "";
    params.config_file = args.count("c_file") ? args.at("c_file") : "";
    params.output_file = args.count("o_file") ? args.at("o_file") : params.mode == TUNE ? "tunned_params" : "validation_output";
    params.output_format = args.count("o_format") ? args.at("o_format") == "long" ? LONG : SHORT : LONG;
    return params;
}

void parameters_validation(SUVMParameters params) {
    if (params.tolerance < 0) {
        throw std::invalid_argument("Tolerance must be non-negative");
    }

    if (params.window_size <= 3) {
        throw std::invalid_argument("Window size must at least 3");
    }

    if (params.invalid_threshold <= 0) {
        throw std::invalid_argument("Invalid threshold must be positive");
    }

    if (params.mode != VALIDATE && params.upper_limit <= params.lower_limit) {
            throw std::invalid_argument("Upper limit must be greater than lower limit");
    }

    if (params.tolerance < 0) {
        throw std::invalid_argument("Tolerance cannot be negative");
    }
}

SUVMParameters parse_args(int argc, char *argv[]) {
    if (argc < 2 || (argc == 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h"))) {
        throw std::invalid_argument(USAGE_EXPLANATION);
    }

    // Parse arguments into a key-value map
    auto args = parse_arguments(argc, argv);
    validate_arguments(args);
    SUVMParameters params = arguments_to_parameters(args);
    parameters_validation(params);
    return params;
}
