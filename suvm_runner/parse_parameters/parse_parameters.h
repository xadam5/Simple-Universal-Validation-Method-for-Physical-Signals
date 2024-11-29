#ifndef PARSE_PARAMETERS_H
#define PARSE_PARAMETERS_H

#include <string>

extern const std::string USAGE_EXPLANATION;

enum ProgramMode {
    TUNE,
    VALIDATE,
    TUNE_VALIDATE
};

enum OutputFormat {
    LONG,
    SHORT
};

struct SUVMParameters {
    ProgramMode mode;
    int window_size;
    float tolerance;
    float lower_limit;
    float upper_limit;
    int invalid_threshold;
    std::string tuning_file;
    std::string validation_file;
    std::string config_file;
    std::string output_file;
    OutputFormat output_format;
};

SUVMParameters parse_args(int argc, char *argv[]);

#endif
