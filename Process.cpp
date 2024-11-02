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
#include <chrono> // Include chrono for timestamp
#include <thread>

Process::Process(const String& processName, const MainConsole::Config& config)
    : processName(processName), config(config) {

    // Get current time
    std::time_t currentTime = std::time(nullptr);

    // Use localtime_s to safely convert time_t to struct tm
    localtime_s(&processCreationTime, &currentTime);

    processContents = StringVector();       // Initialize the process contents (instructions)
    processCurrentInstructionLine = 0;      // Initialize the current instruction line
	processTotalInstructions = generateRandomNumber();  // Initialize the total instructions
	isFinished = false;                     // Initialize the process as not finished
}

void Process::displayProcessInfo() const {
	displayProcessHeader();

    std::cout << "Current Line: " << processCurrentInstructionLine << std::endl;
    std::cout << "Total Instructions: " << processTotalInstructions << std::endl;

    // Display all stored instructions
    for (const auto& instruction : processContents) {
       std::cout << instruction << std::endl;
    }
}

void Process::initProcess() {
    for (int i = 0; i < 50; ++i) {
        std::stringstream ss;
        ss << "Instruction # " << (i + 1) << " for process: " << processName;
        String instruction = ss.str();  // Capture the instruction as a string

        // Store the instruction in the processContents vector
        processContents.push_back(instruction);
		processCurrentInstructionLine += 1;  // Increment the current instruction line

        std::this_thread::sleep_for(std::chrono::milliseconds(200));  // Delay
    }
    isFinished = true;
}

void Process::updateProcessInfo() const
{
    if (!isFinished) {
        displayProcessInfo();
    } else {
        displayProcessHeader();
        std::cout << "Finished!" << std::endl;
    }
}

void Process::displayProcessHeader() const
{
    std::cout << "Process Name: " << processName << std::endl;
	std::cout << "ID: " << std::hash<String>{}(processName) << std::endl;
    std::cout << "Process Creation Time: " << std::put_time(&processCreationTime, "%c") << std::endl;
    std::cout << "\n";

}

// Remove the const here as it modifies processTotalInstructions
uint32_t Process::generateRandomNumber()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(config.min_ins, config.max_ins);

    // set the processTotalInstructions
	return dis(gen);
}

bool Process::getIsFinished() const
{
	return isFinished;
}

String Process::getName() {
    return processName;
}
