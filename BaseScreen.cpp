#include "BaseScreen.h"
#include "TypedefRepo.h"
#include "Process.h"
#include "ConsoleManager.h"
#include "DummyProcessLayout.h"
#include <iostream>
#include <format>


BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) : AConsole(processName), attachedProcess(process) {};

void BaseScreen::onEnabled() {
	std::cout << "Process Name: " << attachedProcess->getName() << std::endl;
	std::cout << " " << std::endl;
}

void BaseScreen::display() {
    printProcessInfo();
}


void BaseScreen::process() {
	printProcessInfo();
	String commandBaseScreen;
    
	while (true) {
		std::cout << "Enter a command: ";
		std::getline(std::cin, commandBaseScreen);

        // Increment the current line of code

		if (commandBaseScreen == "exit") {
			ConsoleManager::getInstance()->exitApplication();			// Exit the application
			ConsoleManager::getInstance()->switchConsole(MAIN_CONSOLE);  // Exit to the previous console
			break;
		}
		if (commandBaseScreen == "process-smi") {

		}
		else {
            // Increment the current instruction line for any command
            attachedProcess->incrementCurrentInstructionLine();

			// Increment the total number of lines of instructions
			attachedProcess->storePrintedLines(commandBaseScreen);

			// display the header with the current process information
			printProcessInfo();

            std::cout << "Command '" << commandBaseScreen << "' is not recognized." << std::endl;
            std::cout << "Current line of instruction: " << attachedProcess->getCurrentInstructionLine() << " / " << attachedProcess->getTotalLinesOfInstructions() << std::endl;
		}
	}
}

// Helper function to print the process information
void BaseScreen::printProcessInfo() const {
	// Dynamically display the process information
	std::cout << "Current line of instruction / Total line of instruction: " << attachedProcess->getCurrentInstructionLine() << " / " << attachedProcess->getTotalLinesOfInstructions() << std::endl;
	std::cout << "Created at: " << attachedProcess->getFormattedTimestamp() << std::endl;
}