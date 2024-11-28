#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include "csv_reader.h"

std::vector<float> readCSV(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        auto error_message = "Error: Could not open the file " + filename;
        throw std::ios_base::failure(error_message);
    }
    
    std::vector<float> data;
    std::string line;
    std::getline(file, line);
    
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
                    std::cerr << "Error: Invalid floating-point number in file:" << cell << std::endl;
                    break;
                }
                break;  // No need to process further in this row once we have the second value
            }
            ++col;  // Move to the next column
        }
    }
    
    file.close();
    return data;
}
