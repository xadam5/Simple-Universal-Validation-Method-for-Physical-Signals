#ifndef LOAD_CONFIG_H
#define LOAD_CONFIG_H

#include "../../validation_method/signal_validator/signal_validator.h"
#include <string>

/**
 * @brief Loads the configuration parameters from a file.
 *
 * This function reads the configuration parameters from the specified file
 * and returns them as a ValidatorParameters object.
 *
 * @param config_file The path to the configuration file.
 * @return ValidatorParameters containing the loaded configuration parameters.
 */
ValidatorParameters load_config(const std::string& config_file);

/**
 * @brief Generates a configuration file with the given parameters.
 *
 * This function writes the provided ValidatorParameters to the specified
 * configuration file.
 *
 * @param params The ValidatorParameters to be written to the file.
 * @param config_file The path to the configuration file.
 */
void generate_config(ValidatorParameters params, const std::string& config_file);

#endif // LOAD_CONFIG_H
