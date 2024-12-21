#ifndef FILE_READER_H
#define FILE_READER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

struct SimulationInput {
    int width = 0;
    int height = 0;
    double g = 0.0;
    double rho = 0.0;
    std::vector<double> initialValues;
};

class FileReader {
public:
    static SimulationInput readFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        SimulationInput input;
        file >> input.width >> input.height;
        file >> input.g >> input.rho;

        input.initialValues.resize(input.width * input.height);
        for (auto& val : input.initialValues) {
            file >> val;
        }

        if (input.width * input.height != static_cast<int>(input.initialValues.size())) {
            throw std::runtime_error("Mismatch between field dimensions and initial values.");
        }

        return input;
    }
};

#endif // FILE_READER_H
