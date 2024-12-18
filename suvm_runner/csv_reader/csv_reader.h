#ifndef CSVREADER_H
#define CSVREADER_H

#include <vector>
#include <string>

/**
 * @brief Reads a CSV file and returns the values as a vector of floats.
 * 
 * This function reads a CSV file specified by the filename and extracts the
 * values, returning them as a vector of floats. It assumes that the CSV file
 * contains two columns of index and float values in this order.
 * 
 * @param filename The name of the CSV file to read.
 * @return A vector of floats containing the values from the CSV file.
 */
std::vector<float> readCSV(const std::string& filename);

#endif // CSVREADER_H
