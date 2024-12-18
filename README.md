# Simple Universal Validation Method (SUVM)

## Overview  
The **Simple Universal Validation Method (SUVM)** is a tool designed for validating one-dimensional sensor data. For each measurement, the validator outputs a status containing flags such as `VALID`, `INVALID`, `OUT_OF_RANGE`, etc. It is flexible and configurable to handle a variety of sensors by tuning its parameters.  

### Features  
The validator can detect the following issues in sensor data:  
- **OUT_OF_RANGE**: The value does not fall inside a predefined range.  
- **MISSING_VALUE**: No value has been measured; the validator can replace it with a prediction.  
- **NOT_CONTINUOUS**: The new value does not follow the continuity of previous values.  
- **STUCK**: The sensor is stuck at one value (with possible small noise).  
- **NOISY**: Increased noise or loss of precision is detected in the sensor.  
- **RAPID_INCREASE**/**RAPID_DECREASE**: Values increase or decrease rapidly.

### Key Highlights  
- **Parameter Tuning**: The method requires tuning of parameters to match the standard behavior of a particular sensor.  
- **Predefined Defaults**: Some parameters have recommended default values, but users may modify them cautiously. Incorrect settings can lead to suboptimal functionality.  

---

## Installation and Build Instructions  
[GCC C++ compiler](https://gcc.gnu.org/) (g++) is required. The program can be built using the `make` command:  
```bash
make
```  
To remove the executable and object files, use:  
```bash
make clean
```
---

## Usage  
The SUVM can operate in three modes:  
1. **Tune**: Determines optimal parameters for the validator and outputs them to a configuration file.  
2. **Validate**: Validates sensor data using a pre-existing configuration file.  
3. **Tune and Validate**: Tunes parameters and uses them to validate sensor data.  

### Command Line Syntax  
```bash
suvm_runner mode=<mode> [options]
```

### Mode-Specific Commands  
#### Tune Mode  
```bash
suvm_runner mode=tune t_file=<path/to/file.csv> l_limit=<num> u_limit=<num> [o_file=<filename> tol=<num> i_thr=<num> w_size=<num>]
```

#### Validate Mode  
```bash
suvm_runner mode=validate v_file=<path/to/file.csv> c_file=<path/to/c_file.ini> [o_file=<filename> o_format=<long|short>]
```

#### Tune and Validate Mode  
```bash
suvm_runner mode=tune_validate t_file=<path/to/file.csv> v_file=<path/to/file.csv> l_limit=<num> u_limit=<num> [o_file=<filename> tol=<num> i_thr=<num> w_size=<num> o_format=<long|short>]
```

### Parameters  
- **`mode`**: Mode of operation (`tune`, `validate`, `tune_validate`).  
- **`w_size`**: Sliding window size (default: 20).  
- **`tol`**: Tolerance for measured thresholds (default: 0.75).  
- **`i_thr`**: Base of threshold for consecutive invalid measurements (default: 10).  
- **`l_limit`**: Lower limit of valid values.  
- **`u_limit`**: Upper limit of valid values.  
- **`t_file`**: Path to the CSV tuning file.  
- **`v_file`**: Path to the CSV validation file.  
- **`c_file`**: Path to the configuration file (INI format).  
- **`o_file`**: Output file name without extension (default: `output_validation`).  
- **`o_format`**: Output format (`long` or `short`; default: `long`).  

---

## File Formats  
### Tuning File  
- **Format**: CSV file with a header line.  
- **Content**: Two columns: `timestamp` (or index) and `value`.  
- **Notes**: The tuning file must contain only valid, anomaly-free data. It should cover the standard behavior of the sensor, its increasing and decreasing trends (if any), noise lever, etc. A minimal number of values is `window_size + 20`; however, much more is recommended.

### Configuration File  
- **Format**: INI file.  
- **Usage**: Generated during the **tune** mode.  
- **Notes**: Manual modifications are discouraged but possible with caution.  

### Validation (Testing) File  
- **Format**: CSV file with the same structure as the tuning file.  
- **Content**: May include invalid values or anomalies only after the initialization phase. Its length depends on the parameters of the validator. The maximal length of the initialization phase is `window_size + 10`. 
- **Initialization Phase**:  
  - Minimum duration: `window_size + 3`.  
  - Maximum duration: `window_size + 10`.  

---

## Modes of Operation  
### 1. Tune  
- Reads a CSV tuning file.  
- Calculates optimal parameters for the validator.  
- Outputs parameters into a configuration file.  

### 2. Validate  
- Loads parameters from a configuration (INI) file.  
- Validates values in a CSV validation file.  
- Outputs results into an output file.  

### 3. Tune and Validate  
- Reads a CSV tuning file.  
- Calculates optimal parameters for the validator.  
- Uses these parameters to validate values in a CSV validation file.  
- Outputs validation results into an output file.  

---

## Examples  
### Tune Mode Example  
```bash
suvm_runner mode=tune t_file=data/tuning_data.csv l_limit=0 u_limit=100
```  

### Validate Mode Example  
```bash
suvm_runner mode=validate v_file=data/validation_data.csv c_file=config.ini
```  

### Tune and Validate Mode Example  
```bash
suvm_runner mode=tune_validate t_file=data/tuning_data.csv v_file=data/validation_data.csv l_limit=0 u_limit=100 o_format=short
```  
