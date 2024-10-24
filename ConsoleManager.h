#pragma once
#include <memory>
#include <vector>
#include "AConsole.h"
#include <unordered_map>
#include <Windows.h>
#include "TypedefRepo.h"										// Contains typedefs for std::string and std::vector<std::string>
#include "BaseScreen.h"

// Major Screens
const String MAIN_CONSOLE = "MAIN_CONSOLE";
const String MARQUEE_CONSOLE = "MARQUEE_CONSOLE";

class ConsoleManager
{
public:
	typedef std::unordered_map<String, std::shared_ptr<AConsole>> ConsoleTable;		// A map of console names to console objects; dictionary; collection of key value pairs

	static ConsoleManager* getInstance();											// Singleton pattern; actual pointer
	static void initialize();														// Initialize the instance of ConsoleManager
	static void destroy();															// Destroy the instance of ConsoleManager

	void drawConsole() const;																// Refreshes the screen with the updated information
	void process() const;																	// Contains handling of logic and other non-drawing operations
	void switchConsole(String consoleName);											// Switches to the specified console
	
	void registerScreen(std::shared_ptr<BaseScreen> screenRef);						// Registers a screen to the console manager
	void switchToScreen(String screenName);											// Switches to the specified screen
	void unregisterScreen(String screenName);										// Unregisters the specified screen
	
	void returnToPreviousConsole();													// Returns to the previous console
	void exitApplication();															// Exits the application
	bool isRunning() const;															// Returns the value of the running variable
	
	HANDLE getConsoleHandle() const;												// Returns the console handle

	void setCursorPosition(int posX, int posY) const;								// Sets the cursor position to the specified coordinates
	void printScreenNames() const;

private:
	ConsoleManager();																// Constructor
	~ConsoleManager() = default;													// Destructor

	ConsoleManager(ConsoleManager const&) {};										// Copy constructor is private
	ConsoleManager& operator=(ConsoleManager const&) {
		return *this; // Ensure to return *this
	};						// Assignment operator is private
	static ConsoleManager* sharedInstance;											// The instance of ConsoleManager

	ConsoleTable consoleTable;														// The table of consoles
	std::shared_ptr<AConsole> currentConsole;										// The current console
	std::shared_ptr<AConsole> previousConsole;										// The previous console

	HANDLE consoleHandle;															// The console handle
	bool running = true;															// The running variable
};

