#ifndef LOAD_CONFIG_H
#define LOAD_CONFIG_H

#include "../signal_validator/signal_validator.h"
#include <string>


ValidatorParameters load_config(const std::string& config_file);
void generate_config(ValidatorParameters params, const std::string& config_file);

#endif
