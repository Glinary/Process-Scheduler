#include "AConsole.h"

// Constructor
AConsole::AConsole(String name) : name(name)
{
}

// Returns the name of the console
String AConsole::getName()
{
	return name;
}