#ifndef PARSE_PARAMETERS_H
#define PARSE_PARAMETERS_H

#include <string>

/**
 * @brief Explanation of the usage of the program.
 */
extern const std::string USAGE_EXPLANATION;

/**
 * @brief Default values for optional parameters.
 */
const int DEFAULT_WINDOW_SIZE = 20;
const float DEFAULT_TOLERANCE = 0.75;
const int DEFAULT_INVALID_THRESHOLD = 10;

/**
 * @enum ProgramMode
 * @brief Defines the modes in which the program can run.
 */
enum ProgramMode
{
    TUNE,           ///< Mode for tuning parameters.
    VALIDATE,       ///< Mode for validating signals.
    TUNE_VALIDATE   ///< Mode for both tuning and validating.
};

/**
 * @enum OutputFormat
 * @brief Defines the output formats for the program.
 */
enum OutputFormat
{
    LONG,   ///< Long format output.
    SHORT   ///< Short format output.
};

/**
 * @struct SUVMParameters
 * @brief Holds the parameters for the SUVM program.
 */
struct SUVMParameters
{
    ProgramMode mode;               ///< Mode in which the program runs.
    int window_size;                ///< Size of the sliding window.
    float tolerance;                ///< Tolerance level for tuning.
    float lower_limit;              ///< Lower limit of the valid range.
    float upper_limit;              ///< Upper limit of the valid range.
    int invalid_threshold;          ///< Threshold for number of invalid values.
    std::string tuning_file;        ///< File with tuning data.
    std::string validation_file;    ///< File with data for validation.
    std::string config_file;        ///< Configuration file.
    std::string output_file;        ///< Output file.
    OutputFormat output_format;     ///< Format of the output.
};


/**
 * @brief Parses command-line arguments to extract SUVM parameters.
 *
 * This function takes the command-line arguments and parses them to extract
 * the parameters required for the SUVM program.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return SUVMParameters containing the parsed parameters.
 */
SUVMParameters parse_args(int argc, char *argv[]);

#endif // PARSE_PARAMETERS_H
