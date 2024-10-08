#include "Process.h"
#include "TypedefRepo.h"
#include <ctime>
#include <sstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <limits>

Process::Process(const String& processName) {
    name = processName;

    // Get current time
    std::time_t currentTime = std::time(nullptr);

    // Use localtime_s to safely convert time_t to struct tm
    localtime_s(&creationTime, &currentTime);  // Pass the current time and store the result in creationTime

    processContents = StringVector();  // Initialize the process contents (instructions)
    currentInstructionLine = 1;  // Initialize the current instruction line
    totalLinesOfInstructions = 1;  // Initialize the total number of instructions (to be set later)
}

// Helper function to store printed lines (instructions)
void Process::storePrintedLines(const String& line) {
    processContents.push_back(line);
    totalLinesOfInstructions++;  // Increment the total number of instructions
}

// Getters
String Process::getName() const {
    return name;
}

// Method to return the formatted timestamp
String Process::getFormattedTimestamp() const {
    std::ostringstream oss;  // Declare ostringstream here to avoid scope issues
    // Format: MM/DD/YYYY, HH:MM:SS AM/PM
    oss << std::put_time(&creationTime, "%m/%d/%Y, %I:%M:%S %p");
    return oss.str();  // Return the formatted timestamp as a string
}

// Method to increment the current instruction line
void Process::incrementCurrentInstructionLine() {
    currentInstructionLine++;
}

// Method to reset the current instruction line to the first line
void Process::resetCurrentInstructionLine() {
    currentInstructionLine = 1;
}

// Getters for current instruction line and total instructions
int Process::getCurrentInstructionLine() const {
    return currentInstructionLine;
}

int Process::getTotalLinesOfInstructions() const {
    return totalLinesOfInstructions;
}
