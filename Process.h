#pragma once
#include "TypedefRepo.h"
#include <string>
#include <ctime>
#include <sstream> 

// Process class is the reference to the process that is being executed in the BaseScreen
class Process
{
public:
	Process(const String& processName);			// Constructor
	
	// Helper functions
	void storePrintedLines(const String& line);	// Store the printed line in the process; starting in the Printed "Enter command: " line
	void incrementCurrentInstructionLine();		// Increment the current instruction line of the process
	void resetCurrentInstructionLine();			// Reset the current instruction line of the process

	// Getters
	String getName() const;						// Get the name of the process
	String getFormattedTimestamp() const;		// Get the formatted timestamp
	int getCurrentInstructionLine() const;		// Get the current line of the process
	int getTotalLinesOfInstructions() const;	// Get the total number of lines in the process
	
private:
	String name;								// The name of the process
	std::tm creationTime;						// The creation time of the process; time when the process was created and cannot be changed
	StringVector processContents;				// The contents of the process; the lines of code in the process; will be inscremented as the process is executed with new lines of code and commands
	int currentInstructionLine;					// The current instruction line of the process; the line of code that is currently being executed
	int totalLinesOfInstructions;				// The total number of lines in the process
};