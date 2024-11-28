// main.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include "sliding_window.h"
#include "../CSVReader.h"

int main() {
    std::vector<double> sensor_data = readCSV("../stuck_data.csv");

    size_t quarterSize = sensor_data.size() / 4;
    std::vector<double> training_data(sensor_data.begin(), sensor_data.begin() + quarterSize); 
    std::vector<double> test_data(sensor_data.begin() + quarterSize, sensor_data.end());

    // const size_t window_size = tuning_window_size(training_data);
    const size_t window_size = 20;
    std::cout << "Optimal window size: " << window_size << std::endl;

    std::vector<double> window_init(test_data.begin(), test_data.begin() + window_size);

    SlidingWindow model(window_size, 0);
    std::vector<bool> validations = {};
    std::vector<double> me_l = {};
    std::vector<double> sd_l = {};

    // Simulate real-time validation
    for (size_t i = window_size; i < test_data.size(); ++i) {
        model.update(test_data[i]);
        bool validation = model.validate();
        validations.push_back(validation);
        me_l.push_back(model.mean());
        sd_l.push_back(model.standard_deviation());
    }


    std::ofstream MyFile("predictions-stuck_data.csv");
    MyFile << ",Temperature,Validation,Mean,Standard_deviation" << std::endl;
    for (size_t i = window_size; i < sensor_data.size(); ++i) {
        double measurement = sensor_data[i];
        int validation = validations[i - window_size] ? -20 : -30;
        double mean = me_l[i - window_size];
        double sd = sd_l[i - window_size];
        MyFile << i - 1 << "," << measurement << "," << validation << "," << mean << "," << sd + 50 << std::endl;
    }

    // Close the file
    MyFile.close();


    return 0;
}

