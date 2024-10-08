#include "ConsoleManager.h"

using namespace std;

int main() {
	ConsoleManager::initialize();												// Initialize the instance of ConsoleManager	

	// Variables
	bool running = true;

	// Main loop
	while (running) {

		ConsoleManager::getInstance()->process();								// contains handling of logic and other non-drawing operations; call its instance
		ConsoleManager::getInstance()->drawConsole();							// refreshes the screen with the updated information

		// Check if still running
		running = ConsoleManager::getInstance()->isRunning();
	}


	// Clean up by destroying the instance of ConsoleManager
	ConsoleManager::destroy();

	return 0;
}