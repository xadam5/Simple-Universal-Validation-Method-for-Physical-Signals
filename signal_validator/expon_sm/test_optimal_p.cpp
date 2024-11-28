// main.cpp
#include <iostream>
#include <fstream>
#include <limits>
#include "../CSVReader.h"
#include "exponential_smooth.h"

// Function to calculate Mean Square Error (MSE)
double calculate_mse_s(const std::vector<double> &actual, const std::vector<double> &predicted) {
    double error_sum = 0.0;
    for (size_t i = 0; i < actual.size(); ++i) {
        error_sum += (actual[i] - predicted[i]) * (actual[i] - predicted[i]);
    }
    return error_sum / actual.size();
}

// Function to find the optimal alpha and beta by testing different combinations
std::vector<double> optimal_parameters_errors(const std::vector<double> &sensor_data, double &best_alpha, double &best_beta) {
    double best_mse = std::numeric_limits<double>::max();  // Start with a large error value
    std::vector<double> predictions;
    std::vector<double> errors;

    for (double alpha = 0.1; alpha <= 0.9; alpha += 0.05) {
        for (double beta = 0.1; beta <= 0.9; beta += 0.05) {
            HoltLinearModel model;
            model.configure(alpha, beta, 10);
            size_t i = 0;
            while (model.isInitializing())
            {
                model.initialize(sensor_data[i]);
                i++;
            }

            predictions.clear();

            // Simulate real-time updates with predictions
            for (; i < sensor_data.size(); ++i) {
                double prediction = model.update(sensor_data[i]);
                predictions.push_back(prediction);
            }

            // Calculate MAE for the current alpha-beta combination
            double mse = calculate_mse_s(std::vector<double>(sensor_data.begin() + 11, sensor_data.end()), predictions);
            if (mse > 500) { mse = 500; }
            errors.push_back(mse);

            if (mse < best_mse) {
                best_mse = mse;
                best_alpha = alpha;
                best_beta = beta;
            }
        }
    }

    std::cout << "Best Alpha: " << best_alpha << "  Best Beta: " << best_beta << std::endl;
    return errors;
}

int main() {
    std::vector<double> sensor_data = readCSV("../KCLT.csv");

    // Initial parameters
    double best_alpha = 0.7;
    double best_beta = 0.7;

    size_t quarterSize = sensor_data.size() / 4;
    std::vector<double> training_data(sensor_data.begin(), sensor_data.begin() + quarterSize);
    
    std::vector<double> test_data_1(sensor_data.begin() + quarterSize, sensor_data.begin() + quarterSize * 2);
    std::vector<double> test_data_2(sensor_data.begin() + quarterSize * 2, sensor_data.begin() + quarterSize * 3);
    std::vector<double> test_data_3(sensor_data.begin() + quarterSize * 3, sensor_data.begin() + quarterSize * 4);

    // Find the optimal alpha and beta based on the sensor data
    std::vector<double> training_errors = optimal_parameters_errors(training_data, best_alpha, best_beta);

    std::vector<double> test_1_errors = optimal_parameters_errors(test_data_1, best_alpha, best_beta);
    std::vector<double> test_2_errors = optimal_parameters_errors(test_data_2, best_alpha, best_beta);
    std::vector<double> test_3_errors = optimal_parameters_errors(test_data_3, best_alpha, best_beta);

    std::ofstream MyFile("optimization_errors.csv");
    MyFile << ",Training_err,Test_1_err,Test_2_err,Test_3_err" << std::endl;
    for (size_t i = 0; i < training_errors.size(); ++i) {
        double error_tr = training_errors[i];
        double error_ts1 = test_1_errors[i];
        double error_ts2 = test_2_errors[i];
        double error_ts3 = test_3_errors[i];
        MyFile << i << "," << error_tr << "," << error_ts1 << "," << error_ts2 << "," << error_ts3 << std::endl;
    }

    // Close the file
    MyFile.close();


    return 0;
}
