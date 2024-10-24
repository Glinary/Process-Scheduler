#pragma once
#include "AConsole.h"
class MainConsole : public AConsole
{
public:
	MainConsole();							// Constructor
	~MainConsole() = default;				// Destructor

	// Override the pure virtual functions of AConsole
	void onEnabled() override;
	void display() override;
	void process() override;

private:
	void ASCIITextHeader() const;						// Prints the ASCII text header
	void displayDevelopers() const;						// Displays the developers of the program
	bool isValidFirstCommand(String command) const;		// Checks if the first command is valid
	bool validateCommand(String& input) const;			// Validates the command
	bool isValidScreenCommand(String command) const;	// Checks if the command is valid
	void recognizeCommand(String command) const;		// Recognizes the command
};
