#include "Process.h"
#include "TypedefRepo.h"
#include <ctime>
#include <sstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <limits>
#include "MainConsole.h"
#include <random>

Process::Process(const String& processName, const MainConsole::Config& config)
    : processName(processName), config(config) {

    // Get current time
    std::time_t currentTime = std::time(nullptr);

    // Use localtime_s to safely convert time_t to struct tm
    localtime_s(&processCreationTime, &currentTime);

    processContents = StringVector();  // Initialize the process contents (instructions)
    processCurrentInstructionLine = 1;  // Initialize the current instruction line
    processTotalInstructions = 0;  // Initialize the total instructions
}

void Process::displayProcessInfo() const {
    std::cout << "Process Name: " << processName << std::endl;
    std::cout << "Current Line: " << processCurrentInstructionLine << std::endl;
	std::cout << "Total Instructions: " << processTotalInstructions << std::endl;

	std::cout << "Min. instrcutions: " << config.min_ins << std::endl;
	std::cout << "Max. instrcutions: " << config.max_ins << std::endl;
}

// Remove the const here as it modifies processTotalInstructions
void Process::generateRandomNumber()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(config.min_ins, config.max_ins);

    // set the processTotalInstructions
    processTotalInstructions = dis(gen);
}
