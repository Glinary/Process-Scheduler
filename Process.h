#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "MainConsole.h"
#include <cstdint>

class Process {
public:
    // Constructor that takes process name and config as parameters
    Process(const String& processName, const MainConsole::Config& config);

    void updateProcessInfo() const;    // Updates the process information
    void displayProcessInfo() const;   // Displays the process information
    void initProcess();                // Initializes the process
    bool getIsFinished() const;        // Checks if the process is finished
    bool getIsOngoing();               // Checks if the process is ongoing
    String getName();                  // Retrieves the process name

private:
    String processName;                // Name of the process
    uint32_t processCurrentInstructionLine; // Current instruction line
    uint32_t processTotalInstructions;      // Total instructions in the process
    std::tm processCreationTime;        // Creation time of the process
    std::vector<String> processContents; // Instructions of the process
    MainConsole::Config config;         // Configuration for the process
    bool isFinished;                    // Flag to check if process is finished
    bool isOngoing;                     // Flag to check if process is ongoing

    void displayProcessHeader() const;  // Displays the process header information
    uint32_t generateRandomNumber();    // Generates a random number
};
