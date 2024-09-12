#include <iostream>
#include <Windows.h>
#include "allfunctions.h"


void ASCIITextHeader() {
	std::cout << "  ____    ____      ___     ____    _______    ____    __   __		" << std::endl;
	std::cout << " / ___|  / ___|    / _ \\   |  _ \\   |  ___|   / ___|   \\ \\ / /	" << std::endl;
	std::cout << "| |      \\___ \\   | | | |  | |_) |  |  __|    \\___ \\    \\ V /	" << std::endl;
	std::cout << "| |___    ___) |  | |_| |  |  __/   | |___     ___) |    | |			" << std::endl;
	std::cout << " \\____|  |____/    \\___/   |_|      |_____|   |____/     |_|		" << std::endl;

	HANDLE console_color;
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_color, 10);
	std::cout << "Hello, Welcome to CSOPESY commandline!" << std::endl;
}

bool isCommandValid(std::string command) {
	if (command == "initialize" || command == "screen" || command == "scheduler-test" || command == "scheduler-stop" || command == "report-util" || command == "clear" || command == "exit") {
		return true;
	}
	else {
		return false;
	}
}

int commandRecognize(std::string command) {
	if (command == "initialize") {
		std::cout << command << " command recognized. Doing something.\n";
		return 0;
	}
	else if (command == "screen") {
		std::cout << command << " command recognized. Doing something.\n";
		return 0;
	}
	else if (command == "scheduler-test") {
		std::cout << command << " command recognized. Doing something.\n";
		return 0;
	}
	else if (command == "scheduler-stop") {
		std::cout << command << " command recognized. Doing something.\n";
		return 0;
	}
	else if (command == "report-util") {
		std::cout << command << " command recognized. Doing something.\n";
		return 0;
	}
	else if (command == "clear") {
		system("CLS");
		return 1;
	}
	else if (command == "exit") {
		/*close the CLI*/
		exit(0);
	}
	else {
		std::cout << "Command not recognized. \n";
		return 0;
	}
}