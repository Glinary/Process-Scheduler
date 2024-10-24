#pragma once
//#include <string>
#include "TypedefRepo.h"

class AConsole										// Abstract class
{
public:
	/*typedef std::string String;*/
	AConsole(String name);
	~AConsole() = default;

	String getName();
	virtual void onEnabled() = 0;					// called when the screen is shown for the first time
	virtual void display() = 0;						// called per frame; for drawing proper
	virtual void process() = 0;						// called when there are certain processes to be done; for input commands; algorithms

	String name;
	friend class ConsoleManager;					// AConsole allows ConsoleManager to access its private members and functions
};

