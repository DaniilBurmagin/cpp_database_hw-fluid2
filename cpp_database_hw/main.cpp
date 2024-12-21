#include <iostream>
#include <string>
#include "Fixed.h"
#include "Simulator.h"
#include "FileReader.h"

int main() {
    try {
        const std::string inputFilePath = "input.txt";

        // ������ ������ �� �����
        SimulationInput input = FileReader::readFromFile(inputFilePath);

        // �������������� ��������� �������� � Fixed<32, 16>
        std::vector<Fixed<32, 16>> fixedInitialValues;
        for (const auto& val : input.initialValues) {
            fixedInitialValues.emplace_back(val);
        }

        // �������� ����������
        FluidSimulator<Fixed<32, 16>> simulator(input.width, input.height);

        // ������������� ����
        simulator.initialize(fixedInitialValues);

        // ������ ��������� �� 10 �����
        for (int step = 0; step < 10; ++step) {
            std::cout << "Simulating step " << step + 1 << "..." << std::endl;
            simulator.simulateStep(input.g, input.rho);
            simulator.saveToFile("output_step_" + std::to_string(step + 1) + ".txt");
        }

        std::cout << "Simulation completed successfully." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
