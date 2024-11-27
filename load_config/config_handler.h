#ifndef LOAD_CONFIG_H
#define LOAD_CONFIG_H

#include <signal_validator.h>
#include <string>


ValidatorParameters load_configuration(const std::string& config_file);
void print_config(ValidatorParameters params);

#endif
