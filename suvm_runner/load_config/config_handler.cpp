#include <limits>
#include "inih/cpp/INIReader.h"
#include <stdexcept>
#include <cmath>
#include "config_handler.h"
#include <iostream>
#include <fstream>

// General template for getting parameters from the config file
template <typename T>
T get_param(const INIReader& reader, const std::string& section, const std::string& name, T default_value) {
    T value = reader.Get(section, name, default_value);
    if (value == default_value && reader.Get(section, name, "") == "") {
        throw std::invalid_argument("Parameter " + section + "." + name + " must be provided");
    }
    return value;
}

// Getting float parameters from the config file
template <>
float get_param(const INIReader& reader, const std::string& section, const std::string& name, float default_value) {
    float value = reader.GetReal(section, name, default_value);
    if (std::isnan(value)) {
        throw std::invalid_argument("Parameter " + section + "." + name + " must be provided");
    }
    return value;
}

// Getting int parameters from the config file
template <>
int get_param(const INIReader& reader, const std::string& section, const std::string& name, int default_value) {
    int value = reader.GetInteger(section, name, default_value);
    if (value == default_value && reader.Get(section, name, "") == "") {
        throw std::invalid_argument("Parameter " + section + "." + name + " must be provided");
    }
    return value;
}

ValidatorParameters load_config(const std::string& config_file) {
    INIReader reader(config_file);
     if (reader.ParseError() < 0) {
        throw std::invalid_argument("Can't load " + config_file);
    }

    auto NONE = std::numeric_limits<float>::quiet_NaN();

    SmoothingParameters smoothing_params;
    smoothing_params.alpha = get_param(reader, "exp_smoothing", "alpha", NONE);
    smoothing_params.beta = get_param(reader, "exp_smoothing", "beta", NONE);
    smoothing_params.max_asc = get_param(reader, "exp_smoothing", "max_asc", NONE);
    smoothing_params.max_des = get_param(reader, "exp_smoothing", "max_des", NONE);

    WindowParameters window_params;
    window_params.window_size = get_param(reader, "sliding_window", "window_size", static_cast<int>(NONE));
    window_params.min_sd = get_param(reader, "sliding_window", "min_sd", NONE);
    window_params.max_sd = get_param(reader, "sliding_window", "max_sd", NONE);
    window_params.max_z_score = get_param(reader, "sliding_window", "max_z_score", NONE);

    ValidatorParameters params;
    params.smoothing_params = smoothing_params;
    params.window_params = window_params;
    params.range_l_limit = get_param(reader, "range", "lower_limit", NONE);
    params.range_u_limit = get_param(reader, "range", "upper_limit", NONE);
    params.invalid_threshold = get_param(reader, "exp_smoothing", "invalid_threshold", static_cast<int>(NONE));

    return params;
}

void generate_config(ValidatorParameters params, const std::string& config_file) {
    std::ofstream ini_file(config_file + ".ini");
    ini_file << "[exp_smoothing]" << std::endl;
    ini_file << "alpha = " << params.smoothing_params.alpha << std::endl;
    ini_file << "beta = " << params.smoothing_params.beta << std::endl;
    ini_file << "max_asc = " << params.smoothing_params.max_asc << std::endl;
    ini_file << "max_des = " << params.smoothing_params.max_des << std::endl;
    ini_file << "invalid_threshold = " << params.invalid_threshold << std::endl;
    ini_file << std::endl;

    ini_file << "[sliding_window]" << std::endl;
    ini_file << "window_size = " << params.window_params.window_size << std::endl;
    ini_file << "min_sd = " << params.window_params.min_sd << std::endl;
    ini_file << "max_sd = " << params.window_params.max_sd << std::endl;
    ini_file << "max_z_score = " << params.window_params.max_z_score << std::endl;
    ini_file << std::endl;

    ini_file << "[range]" << std::endl;
    ini_file << "lower_limit = " << params.range_l_limit << std::endl;
    ini_file << "upper_limit = " << params.range_u_limit << std::endl;
    ini_file.close();
}