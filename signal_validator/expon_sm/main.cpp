// main.cpp
#include <iostream>
#include <fstream>
#include "../data_manipulation/CSVReader.h"
#include "exponential_smooth.h"

int main() {
    std::vector<double> sensor_data = readCSV("../variable-noise_temperature_data.csv");

    // Initial parameters
    double best_alpha = 0.7;
    double best_beta = 0.7;

    size_t quarterSize = sensor_data.size() / 4;
    std::vector<double> training_data(sensor_data.begin(), sensor_data.begin() + quarterSize);
    
    std::vector<double> test_data(sensor_data.begin() + quarterSize, sensor_data.end());

    // Find the optimal alpha and beta based on the sensor data
    find_optimal_parameters(training_data, best_alpha, best_beta);

    std::cout << "Optimal Alpha: " << best_alpha << ", Optimal Beta: " << best_beta << std::endl;

    // Now use the model with the optimized parameters for real-time prediction
    HoltLinearModel model;
    size_t i = 0;
    model.configure(best_alpha, best_beta, 20);
    while (model.isInitializing())
    {
        model.initialize(test_data[i]);
        i++;
    }

    if (i != 20) {
        std::cerr << "Initializing phase should took 20 steps, but ended after" << i + 1 << std::endl;
    }
    

    std::vector<double> predictions;

    // Simulate real-time updates
    for (; i < test_data.size(); ++i) {
        double prediction = model.update(test_data[i]);
        predictions.push_back(prediction);
    }


    std::ofstream MyFile("simulations/init_tune-variable-noise_temperature_data.csv");
    MyFile << ",Actual,Prediction,Difference" << std::endl;
    for (size_t i = 0; i + 21 < test_data.size(); ++i) {
        double measurement = test_data[i + 21];
        double prediction = predictions[i];
        MyFile << i << "," << measurement << "," << prediction << "," << (measurement - prediction) << std::endl;
    }

    // Close the file
    MyFile.close();


    return 0;
}

