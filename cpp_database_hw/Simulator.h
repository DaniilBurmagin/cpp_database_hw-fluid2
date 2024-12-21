#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>

template <typename ValueType>
class FluidSimulator {
    int width;
    int height;
    std::vector<std::vector<ValueType>> field;

public:
    FluidSimulator(int w, int h) : width(w), height(h), field(h, std::vector<ValueType>(w)) {}

    void initialize(const std::vector<ValueType>& initialValues) {
        if (initialValues.size() != width * height) {
            throw std::runtime_error("Initial values size does not match field dimensions.");
        }
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                field[i][j] = initialValues[i * width + j];
                std::cout << "Initialized field[" << i << "][" << j << "] = " << field[i][j].toDouble() << std::endl;
            }
        }
    }

    void simulateStep(double g, double rho) {
        std::vector<std::vector<ValueType>> newField(height, std::vector<ValueType>(width));

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                ValueType sum = field[i][j];
                int count = 1;

                if (i > 0) {
                    sum = sum + field[i - 1][j];
                    ++count;
                }
                if (i < height - 1) {
                    sum = sum + field[i + 1][j];
                    ++count;
                }
                if (j > 0) {
                    sum = sum + field[i][j - 1];
                    ++count;
                }
                if (j < width - 1) {
                    sum = sum + field[i][j + 1];
                    ++count;
                }

                newField[i][j] = (sum + ValueType(g * rho)) / ValueType(count);

                std::cout << "Updated field[" << i << "][" << j << "] = " << newField[i][j].toDouble() << std::endl;
            }
        }

        field = newField;
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for saving: " + filename);
        }

        for (const auto& row : field) {
            for (const auto& val : row) {
                file << val.toDouble() << " ";
            }
            file << "\n";
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for loading: " + filename);
        }

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                double value;
                file >> value;
                field[i][j] = ValueType(value);
                std::cout << "Loaded field[" << i << "][" << j << "] = " << value << std::endl;
            }
        }
    }

    void saveState(const std::string& filename) const {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for saving state: " + filename);
        }

        for (const auto& row : field) {
            file.write(reinterpret_cast<const char*>(row.data()), sizeof(ValueType) * width);
        }
    }

    void loadState(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for loading state: " + filename);
        }

        for (auto& row : field) {
            file.read(reinterpret_cast<char*>(row.data()), sizeof(ValueType) * width);
        }
    }

    void printField() const {
        for (const auto& row : field) {
            for (const auto& val : row) {
                std::cout << val.toDouble() << " ";
            }
            std::cout << "\n";
        }
    }
};

#endif // SIMULATOR_H
