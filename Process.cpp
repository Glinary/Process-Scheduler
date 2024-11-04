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
#include "Scheduler.h"

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
    coreNum = 0;
    timeFinished = 0;
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

void Process::initProcess(int key) {
    coreNum = key;                      // assign core number
    int i = 0;
    int cycle = 0;

    isOngoing = true;
    while (processCurrentInstructionLine < 50) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));


        if (config.delays_per_exec == 0) {
            cycle = Scheduler::getInstance()->getCycle();
            String instruction = "Instruction #" + std::to_string(i+1) + "for " + processName + std::to_string(cycle);

            // Store the instruction in the processContents vector
            processContents.push_back(instruction);
            processCurrentInstructionLine += 1;
            i++;
        }
        else {
            
            if (Scheduler::getInstance()->getCycle() % config.delays_per_exec == 0){
                cycle = Scheduler::getInstance()->getCycle();
                String instruction = "Instruction #" + std::to_string(i+1) + "for " + processName + std::to_string(cycle);

                // Store the instruction in the processContents vector
                processContents.push_back(instruction);
                processCurrentInstructionLine += 1;
                i++;
            }
        }
    }
    timeFinished = cycle;
    isFinished = true;
    isOngoing = false; 
}

void Process::DisplayUpdate() {
    std::cout << processName + "   " + std::to_string(coreNum)  << std::endl;
}

void Process::updateProcessInfo() const
{
    if (!isFinished) {
        displayProcessInfo();
    } else {
        displayProcessHeader();
        std::cout << "Finished! by " + std::to_string(timeFinished) << std::endl;
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

bool Process::getIsOngoing()
{
	return isOngoing;
}

String Process::getName() {
    return processName;
}

int Process::getTimeFinished() {
    return timeFinished;
}

void Process::setCore(int core){
    coreNum = core;
}