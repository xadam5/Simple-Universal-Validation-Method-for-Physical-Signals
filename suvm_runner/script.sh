#!/bin/bash

g++ -o suvm suvm_runner.cpp ../signal_validator/signal_validator.cpp ../validator_handler/validator_handler.cpp ../auto_tuning/auto_tuning.cpp ../signal_validator/expon_sm/exponential_smooth.cpp ../signal_validator/sliding_wind/sliding_window.cpp ../data_manipulation/CSVReader.cpp parse_parameters/parse_parameters.cpp load_config/config_handler.cpp load_config/inih/cpp/INIReader.cpp load_config/inih/ini.c  -std=c++11
./suvm