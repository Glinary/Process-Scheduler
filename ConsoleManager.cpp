#include "ConsoleManager.h"
#include <iostream>
#include "MainConsole.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
using HANDLE = int; // Placeholder for HANDLE on non-Windows systems
#endif

ConsoleManager* ConsoleManager::sharedInstance = nullptr; // Initialize the instance of ConsoleManager

ConsoleManager* ConsoleManager::getInstance() { // Singleton pattern; actual pointer
	return sharedInstance;
}

void ConsoleManager::initialize() {
	sharedInstance = new ConsoleManager(); // Initialize the instance of ConsoleManager
}

void ConsoleManager::destroy() {
	delete sharedInstance; // Destroy the instance of ConsoleManager
}

void ConsoleManager::drawConsole() const {
	if (this->currentConsole != nullptr) {
		this->currentConsole->display();
	} else {
		std::cerr << "No console to display. Please check." << std::endl;
	}
}

void ConsoleManager::process() const {
	if (this->currentConsole != nullptr) {
		this->currentConsole->process();
	} else {
		std::cerr << "No console to process. Please check." << std::endl;
	}
}

void ConsoleManager::switchConsole(String consoleName) {
	if (this->consoleTable.contains(consoleName)) {
		#ifdef _WIN32
		system("cls"); // Clear the screen on Windows
		#else
		system("clear"); // Clear the screen on macOS/Linux
		#endif
		this->previousConsole = this->currentConsole; // Set the previous console to the current console
		this->currentConsole = this->consoleTable[consoleName]; // Set the current console to the specified console
		this->currentConsole->onEnabled(); // Call the onEnabled function of the current console
	} else {
		std::cerr << "Console name " << consoleName << " not found. Please check if it is initialized." << std::endl;
	}
}

void ConsoleManager::registerScreen(std::shared_ptr<BaseScreen> screenRef) { // stores the screen to the console table
	if (this->consoleTable.contains(screenRef->getName())) {
		std::cerr << "Screen name " << screenRef->getName() << " already exists. Please use another name." << std::endl;
		return;
	}
	this->consoleTable[screenRef->getName()] = screenRef;
}

void ConsoleManager::switchToScreen(String screenName) { // switches to the specified screen
	if (this->consoleTable.contains(screenName)) {
		#ifdef _WIN32
		system("cls"); // Clear the screen on Windows
		#else
		system("clear"); // Clear the screen on macOS/Linux
		#endif
		this->previousConsole = this->currentConsole; // Set the previous console to the current console
		this->currentConsole = this->consoleTable[screenName]; // Set the current console to the specified console
		this->currentConsole->onEnabled(); // Call the onEnabled function of the current console
	} else {
		std::cerr << "Screen name " << screenName << " not found. Please check if it is initialized." << std::endl;
	}
}

void ConsoleManager::unregisterScreen(String screenName) {
	if (this->consoleTable.contains(screenName)) {
		this->consoleTable.erase(screenName);
	} else {
		std::cerr << "Unable to unregister screen name " << screenName << " since it was not found. Please check if it is registered." << std::endl;
	}
}

ConsoleManager::ConsoleManager()
{
	this->running = true; // Set the running variable to true
	#ifdef _WIN32
	this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); // Get the console handle on Windows
	#else
	this->consoleHandle = 0; // Placeholder handle for macOS/Linux
	#endif

	const std::shared_ptr<MainConsole> mainConsole = std::make_shared<MainConsole>();
	this->consoleTable[MAIN_CONSOLE] = mainConsole; // Add the main console to the console table
	this->switchConsole(MAIN_CONSOLE); // Switch to the main console since it is the first console to be displayed; main menu
}

void ConsoleManager::returnToPreviousConsole() {
	if (this->previousConsole != nullptr) {
		#ifdef _WIN32
		system("cls"); // Clear the screen on Windows
		#else
		system("clear"); // Clear the screen on macOS/Linux
		#endif
		this->currentConsole = this->previousConsole; // Set the current console to the previous console
		this->previousConsole = nullptr; // Set the previous console to null
		this->currentConsole->onEnabled(); // Call the onEnabled function of the current console
	} else {
		std::cerr << "No previous console to return to. Please check." << std::endl;
	}
}

void ConsoleManager::exitApplication() {
	this->running = false; // Set the running variable to false
}

bool ConsoleManager::isRunning() const {
	return this->running; // Returns the value of the running variable
}

HANDLE ConsoleManager::getConsoleHandle() const {
	return this->consoleHandle; // Returns the console handle
}

void ConsoleManager::setCursorPosition(int posX, int posY) const {
	#ifdef _WIN32
	COORD coord;
	coord.X = posX;
	coord.Y = posY;
	SetConsoleCursorPosition(this->consoleHandle, coord);
	#else
	std::cout << "\033[" << posY << ";" << posX << "H"; // ANSI escape code for cursor position
	#endif
}

void ConsoleManager::printScreenNames() const {
    if (consoleTable.size() <= 1 && consoleTable.contains(MAIN_CONSOLE)) {
        std::cerr << "There are no screens created." << std::endl;
        return;
    }
    int count = 0;
    for (const auto& entry : consoleTable) {
        if (entry.first != MAIN_CONSOLE) {
            std::cout << ++count << ": " << entry.first << std::endl;
        }
    }
}

bool ConsoleManager::isScreenRegistered(String screenName) const {
	return consoleTable.contains(screenName);
}

std::vector<std::shared_ptr<Process>> ConsoleManager::getProcesses(){
	return this->processes;
}

void ConsoleManager::addProcesses(std::shared_ptr<Process> process){
	this->processes.push_back(process);
}
