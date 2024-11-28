// Specialization for float
#include <limits>
#include "inih/cpp/INIReader.h"
#include <stdexcept>
#include <cmath>
#include "config_handler.h"
#include <iostream>
#include <fstream>

// General template for other types (if needed)
template <typename T>
T get_param(const INIReader& reader, const std::string& section, const std::string& name, T default_value) {
    T value = reader.Get(section, name, default_value);
    if (value == default_value && reader.Get(section, name, "") == "") {
        throw std::invalid_argument("Parameter " + section + "." + name + " must be provided");
    }
    return value;
}

template <>
float get_param(const INIReader& reader, const std::string& section, const std::string& name, float default_value) {
    float value = reader.GetReal(section, name, default_value);
    if (std::isnan(value)) {
        throw std::invalid_argument("Parameter " + section + "." + name + " must be provided");
    }
    return value;
}

// Specialization for int
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

    SmoothingParameters smooth_param;
    smooth_param.alpha = get_param(reader, "exp_smoothing", "alpha", NONE);
    smooth_param.beta = get_param(reader, "exp_smoothing", "beta", NONE);
    smooth_param.max_asc = get_param(reader, "exp_smoothing", "max_asc", NONE);
    smooth_param.max_des = get_param(reader, "exp_smoothing", "max_des", NONE);

    WindowParameters window_param;
    window_param.window_size = get_param(reader, "sliding_window", "window_size", static_cast<int>(NONE));
    window_param.min_sd = get_param(reader, "sliding_window", "min_sd", NONE);
    window_param.max_sd = get_param(reader, "sliding_window", "max_sd", NONE);
    window_param.threshold = get_param(reader, "sliding_window", "continuity_threshold", NONE);

    ValidatorParameters params;
    params.smooth_param = smooth_param;
    params.window_param = window_param;
    params.range_l_limit = get_param(reader, "range", "lower_limit", NONE);
    params.range_u_limit = get_param(reader, "range", "upper_limit", NONE);

    return params;
}

void generate_config(ValidatorParameters params, const std::string& config_file) {
    std::ofstream ini_file(config_file + ".ini");
    ini_file << "[exp_smoothing]" << std::endl;
    ini_file << "alpha = " << params.smooth_param.alpha << std::endl;
    ini_file << "beta = " << params.smooth_param.beta << std::endl;
    ini_file << "max_asc = " << params.smooth_param.max_asc << std::endl;
    ini_file << "max_des = " << params.smooth_param.max_des << std::endl;
    ini_file << std::endl;

    ini_file << "[sliding_window]" << std::endl;
    ini_file << "window_size = " << params.window_param.window_size << std::endl;
    ini_file << "min_sd = " << params.window_param.min_sd << std::endl;
    ini_file << "max_sd = " << params.window_param.max_sd << std::endl;
    ini_file << "continuity_threshold = " << params.window_param.threshold << std::endl;
    ini_file << std::endl;

    ini_file << "[range]" << std::endl;
    ini_file << "lower_limit = " << params.range_l_limit << std::endl;
    ini_file << "upper_limit = " << params.range_u_limit << std::endl;
    ini_file.close();
}