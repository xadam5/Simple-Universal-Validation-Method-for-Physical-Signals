# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -g

# Include directories
INCLUDES = -Iload_config/inih/cpp

# Source files
SRCS = suvm_runner.cpp \
		../signal_validator/signal_validator.cpp \
		../validator_handler/validator_handler.cpp \
		../auto_tuning/auto_tuning.cpp \
		../signal_validator/expon_sm/exponential_smooth.cpp \
		../signal_validator/sliding_wind/sliding_window.cpp \
		csv_reader/csv_reader.cpp \
		parse_parameters/parse_parameters.cpp \
		load_config/config_handler.cpp \
		load_config/inih/cpp/INIReader.cpp \
		load_config/inih/ini.c

# Object files
OBJS = $(SRCS:.cpp=.o) $(SRCS:.c=.o)

# Executable name
EXEC = suvm

# Default target
all: $(EXEC)

# Link the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile .c source files into object files
%.o: %.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build files
clean:
	rm -f $(EXEC) $(OBJS)

# Phony targets
.PHONY: all clean