#!/bin/bash

g++ -o suvm suvm_runner.cpp ../signal_validator/signal_validator.cpp ../validator_handler/validator_handler.cpp ../auto_tuning/auto_tuning.cpp ../expon_sm/exponential_smooth.cpp ../sliding_wind/sliding_window.cpp ../data_manipulation/CSVReader.cpp -std=c++11
./suvm