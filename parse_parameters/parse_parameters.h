#ifndef PARSE_PARAMETERS_H
#define PARSE_PARAMETERS_H

#include <vector>
#include <string>

enum Mode {
    TUNE,
    VALIDATE,
    TUNE_VALIDATE
};

enum OutputFormat {
    LONG,
    SHORT
};

struct SUVMParameters {
    Mode mode;
    size_t window_size;
    float threshold;
    float range_l_limit;
    float range_u_limit;
    std::string training_file;
    std::string test_file;
    std::string validation_file;
    std::string output_file;
    OutputFormat output_format;
};

SUVMParameters parse_args(int argc, char *argv[]);

#endif
