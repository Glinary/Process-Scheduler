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

    void updateProcessInfo() const;
	void displayProcessInfo() const;

private:
    String processName;
    uint32_t processCurrentInstructionLine;
	uint32_t processTotalInstructions;
    std::tm processCreationTime;
    std::vector<String> processContents;
    MainConsole::Config config;
    bool isFinished;
    
	void displayProcessHeader() const;
    uint32_t generateRandomNumber();
};
