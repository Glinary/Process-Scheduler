// Process.h
#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "MainConsole.h"
#include <cstdint>


class Process {
public:
    // Add a new constructor that takes Config as a parameter
    Process(const String& processName, const MainConsole::Config& config);

    void updateProcessInfo() const;
	void displayProcessInfo() const;
    void initProcess(int key);

    void DisplayUpdate();
    bool getIsFinished() const;
    bool getIsOngoing();
    String getName();
    int getTimeFinished();
    void executeInstruction(int key);
    void setCore(int core);

private:
    int coreNum;
    String processName;
    uint32_t processCurrentInstructionLine;
	uint32_t processTotalInstructions;
    std::tm processCreationTime;
    std::vector<String> processContents;
    MainConsole::Config config;
    bool isFinished;
    bool isOngoing = false;
    int timeFinished;


	void displayProcessHeader() const;
    uint32_t generateRandomNumber();
};
