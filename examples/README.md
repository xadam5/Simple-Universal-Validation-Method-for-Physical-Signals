# Validation System Demonstration and Used Datasets 

## Example Scenarios
The following examples demonstrate the functionality of the validation method using various datasets. In each folder, there are CSV files (and corresponding plots) containing data for tuning, testing, and validation output from the validation method.  

### 1. Real Data: Bearing Monitoring System  
- **Description**: Tuning and testing data are sourced from the [Bearing Monitoring System](#bearing-monitoring-system) sensors measuring different axes.
- **Adjustment**: Increased the tolerance value to `0.8` (default is `0.75`) to handle the noise.  
- **Outcome**: The validator detects outliers using the `NOT_CONTINUOUS` flag and remains unaffected by these anomalies.  

---

### 2. Real Data: Angle of Arrival (AoA) Sensor  
- **Description**: Data originate from an [Angle of Arrival (AoA) sensor](#uwb-dual-antenna-aoa-sensor). The testing data includes a section with higher noise.  
- **Adjustment**: Increased the tolerance value to `0.8` (default is `0.75`) to handle the noise.  
- **Use Case**: Useful when tuning data does not fully capture all potential conditions (e.g., varying noise levels).  

---

### 3. Incorrect Tuning  
- **Description**:  
  - Data originate from the [Bearing Monitoring System](#bearing-monitoring-system) sensor measuring different axes.  
  - Tuning data are more static while testing data include a valid trend change.  
- **Outcome**: The validator incorrectly flags the trend change as invalid due to inappropriate tuning data.

---

### 4. Missing Values  
- **Description**: Modified data from the [AoA sensor](#uwb-dual-antenna-aoa-sensor) with several values removed from the testing dataset.  
- **Outcome**:  
  - The validator successfully recovers from single or multiple missing values.  
  - For a longer sequence of missing values, the validator demonstrates its limitations in recovery.  

---

### 5. Noise Change  
- **Description**: Synthetic data generated using the [Mandrova software](https://github.com/makinarocks/Mandrova).  
- **Scenario**:  
  - Tuning data: Contains valid data with consistent noise levels.  
  - Testing data:  
    - Initially matches the tuning data's noise levels.  
    - Later exhibits increased noise levels.  
- **Outcome**: The validator detects the increased noise using the `NOISY` flag.  

---

### 6. Stuck Data  
- **Description**: [temperature measurement data from the US](#us-weather-history).  
- **Scenario**:  
  - Tuning data: Contains valid readings.  
  - Testing data: Contains valid readings followed by a simulated stuck sensor with a small amount of noise.  
- **Outcome**: The validator identifies the sensor as stuck using the `STUCK` flag.  

---

### 7. Outliers  
- **Description**: Modified data from the [Bearing Monitoring System](#bearing-monitoring-system) sensor.  
- **Scenario**:  
  - Testing data includes a short series of significantly higher/lower values (outliers).  
- **Outcome**: The validator detects the inconsistency using the `NOT_CONTINUOUS` flag and recovers seamlessly from the anomaly.  

---  

These scenarios showcase the validator's versatility and accuracy in handling various data challenges. Proper tuning remains essential for achieving optimal results.

## Source of Datasets
### [UWB Dual Antenna AoA sensor](https://github.com/ETH-PBL/UWB_DualAntenna_AoA)
#### Sensor Description
The UWB Dual Antenna AoA (Angle of Arrival) sensor is designed to measure precise angles and distances. It utilizes two antennas to capture signals and calculate AoA and distance using techniques like Time Difference of Arrival (TDOA) and Phase Difference of Arrival (PDOA). The sensor is evaluated in different environments and configurations to ensure accurate and stable measurements.

#### TDOA (Time Difference of Arrival)
TDOA measures the difference in arrival times of a signal at two different antennas. This difference is used to calculate the distance between the source and the sensor. In this repository, TDOA values are represented in units of 15.65 picoseconds, providing high precision in distance measurement.

For more detailed information, refer to the [Preprocessed Measurement Data README](https://github.com/ETH-PBL/UWB_DualAntenna_AoA/blob/main/Dataset/Preprocessed%20Measurement%20Data/README.md).

### [Bearing Monitoring System](https://www.kaggle.com/datasets/isaienkov/bearing-classification)
This dataset focuses on integrating AutoML solutions for bearing condition monitoring. Signals were collected to determine if bearings are whole or defective. For the experiments, two bearings were installed on a shaft. The rotation speed varied from 0 to 1500 rpm, held for 10 seconds, and then decreased to 250 rpm. A DC motor drove the shaft through a coupling, and a radial load of 3.5 kg was applied using a balanced weight. GY-61 ADXL335 accelerometers were mounted on the bearing housing to record signals along the x, y, and z axes.

### [US Weather History](https://github.com/fivethirtyeight/data/tree/master/us-weather-history)
This dataset contains temperature measurements across the US. Each file corresponds to US city ICAO airport codes.

---

## Plot Validation Output
For generating charts from validation outputs of this method, I recommend one of these approaches:

### Static Plot Image
For generating static plots (as PNG file), [Python script](plot_static.py) can be used.
```bash
python3 plot_validation.py <path/csv_filename_without_extension>
```
Required packages: Pandas, Matplotlib.
```bash
pip install pandas matplotlib
```

### Interactive Plot
For generating interactive plots (as HTML file), [Python script](plot_interactive.py) can be used.
```bash
python3 plot_interactive.py <path/csv_filename_without_extension>
```
Required packages: Pandas, Plotly, Kaleido
```bash
pip install pandas plotly kaleido
```

### Online CSV tools
Alternatively, online CSV file analyzers may be used, for instance, at https://chart-studio.plotly.com/create/#/.

