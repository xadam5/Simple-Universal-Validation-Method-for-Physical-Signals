# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -g

# Include directories
INCLUDES = -Isuvm_runner/load_config/inih/cpp

# Source files
SRCS_CPP = suvm_runner/suvm_runner.cpp \
		validation_method/signal_validator/signal_validator.cpp \
		validation_method/validator_handler/validator_handler.cpp \
		validation_method/auto_tuning/auto_tuning.cpp \
		validation_method/signal_validator/exponential_smoothing/exponential_smoothing.cpp \
		validation_method/signal_validator/sliding_window/sliding_window.cpp \
		suvm_runner/csv_reader/csv_reader.cpp \
		suvm_runner/parse_parameters/parse_parameters.cpp \
		suvm_runner/load_config/config_handler.cpp \
		suvm_runner/load_config/inih/cpp/INIReader.cpp

SRCS_C = suvm_runner/load_config/inih/ini.c

# Object files
OBJS_CPP = $(SRCS_CPP:.cpp=.o)
OBJS_C = $(SRCS_C:.c=.o)

# All object files
OBJS = $(OBJS_CPP) $(OBJS_C)

# Executable name
EXEC = suvm

# Default target
all: $(EXEC)

# Link the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(OBJS)

# Compile C++ source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile C source files into object files
%.o: %.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build files
clean:
	rm -f $(EXEC) $(OBJS)

# Phony targets
.PHONY: all clean
