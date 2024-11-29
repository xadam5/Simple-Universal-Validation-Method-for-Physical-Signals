#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>
#include "csv_reader.h"

std::vector<float> readCSV(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        auto error_message = "Error: Could not open the file " + filename;
        throw std::ios_base::failure(error_message);
    }
    
    std::vector<float> data;
    std::string line;
    std::getline(file, line); // Skip the header
    
    // Read the file line by line
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;  // Track the column index
        
        // Process each value in the row
        while (std::getline(ss, cell, ',')) {
            if (col == 1) {  // We're interested in the second column (index 1)
                try {
                    data.push_back(std::stod(cell));  // Convert to float and store
                } catch (const std::invalid_argument& e) {
                    data.push_back(std::numeric_limits<float>::quiet_NaN());  // Store NaN if conversion fails
                    break;
                }
                break;
            }
            ++col;
        }
    }
    
    file.close();
    return data;
}
