// Process.h
#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "MainConsole.h"

class Process {
public:
    // Add a new constructor that takes Config as a parameter
    Process(const String& processName, const MainConsole::Config& config);

    // Existing methods
	void displayProcessInfo() const;

    void generateRandomNumber();

private:
    String processName;
    uint32_t processCurrentInstructionLine;
	uint32_t processTotalInstructions;
    std::tm processCreationTime;
    std::vector<String> processContents;

    // Add a member variable to hold the config
    MainConsole::Config config;
};
