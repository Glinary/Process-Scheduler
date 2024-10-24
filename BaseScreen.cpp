#include "BaseScreen.h"
#include "TypedefRepo.h"
#include "Process.h"
#include "ConsoleManager.h"
#include "DummyProcessLayout.h"
#include <iostream>
#include <format>


BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) : AConsole(processName), attachedProcess(process) {};

void BaseScreen::onEnabled() {
	attachedProcess->displayProcessInfo();
}

void BaseScreen::display() {
	/*if (!refreshed) {
		refreshed = true;
		std::cout << "Process Name: " << attachedProcess->getName() << std::endl;
		std::cout << " " << std::endl;
		printProcessInfo();
	}*/

	std::cout << "Temporarily displaying the process layout" << std::endl;
}


void BaseScreen::process() {
	String commandBaseScreen;
    
	while (true) {
		std::cout << "\nEnter a command: ";
		std::getline(std::cin, commandBaseScreen);
		std::cout << "\n";

        // Increment the current line of code

		if (commandBaseScreen == "exit") {
			ConsoleManager::getInstance()->exitApplication();			// Exit the application
			ConsoleManager::getInstance()->switchConsole(MAIN_CONSOLE);  // Exit to the previous console
			break;
		}
		if (commandBaseScreen == "process-smi") {
			attachedProcess->displayProcessInfo();
		}
		else {
			std::cout << "Command not recognized. Please try again." << std::endl;
		}
	}
}