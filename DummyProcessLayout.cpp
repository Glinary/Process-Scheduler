#include "DummyProcessLayout.h"
#include "ConsoleManager.h"
#include "TypedefRepo.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <Windows.h>

// Constructor: Set the name of the console when DummyLayout is instantiated
DummyProcessLayout::DummyProcessLayout(std::shared_ptr<Process> process, String processName) : AConsole(processName) {}

// Override of onEnabled: This is called when the screen is shown for the first time
void DummyProcessLayout::onEnabled() {
	displayCurrentDateTimes();
	displayGPUSummaries();

	// Create dummy GPU process data
	std::vector<GPUProcess> processes = {
		{0, "N/A", "N/A", 1368, "C+G", "C:\\Windows\\System32\\dwm.exe", "N/A"},
		{0, "N/A", "N/A", 2116, "C+G", "...wekyb3d8bbwe\\XboxGameBarWidgets.exe", "N/A"},
		{0, "N/A", "N/A", 4224, "C+G", "...on\\123.0.2420.65\\msedgewebview2.exe", "N/A"},
		{0, "N/A", "N/A", 5684, "C+G", "C:\\Windows\\explorer.exe", "N/A"},
		{0, "N/A", "N/A", 6676, "C+G", "...nt.CBS_cw5n1h2txyewy\\SearchHost.exe", "N/A"},
		{0, "N/A", "N/A", 6700, "C+G", "...2txyewy\\StartMenuExperienceHost.exe", "N/A"}
	};

	// Display the process list
	displayProcessList(processes);
}

// Override of display: Called to draw the screen each frame
void DummyProcessLayout::display() {
	displayCurrentDateTimes();
	displayGPUSummaries();
}

// Override of process: Handle input commands or other processes here
void DummyProcessLayout::process() {
	String command;
	bool isValidCommand;

	while (true) {
		std::cout << "Enter a command: ";
		std::getline(std::cin, command);

		isValidCommand = validateCommand(command);

		// If the command is recognized
		if (command == "exit") {
			// get back to the previous console (main console)
			ConsoleManager::getInstance()->returnToPreviousConsole();
		}
	}
}


// Helper function to validate the command
bool DummyProcessLayout::validateCommand(String& input) const {
	if (input == "exit") {
		return true;
	}
	else {
		return false;
	}
}

void DummyProcessLayout::displayGPUSummaries() const
{
	std::cout << "+-----------------------------------------------------------------------------------------+" << std::endl;
	std::cout << "| NVIDIA-SMI 551.86                 Driver Version: 551.86         CUDA Version: 12.4     |" << std::endl;
	std::cout << "|-----------------------------------------+------------------------+----------------------+" << std::endl;
	std::cout << "| GPU  Name                     TCC/WDDM  | Bus-Id          Disp.A | Volatile Uncorr. ECC |" << std::endl;
	std::cout << "| Fan  Temp   Perf          Pwr:Usage/Cap |           Memory-Usage | GPU-Util  Compute M. |" << std::endl;
	std::cout << "|                                         |                        |               MIG M. |" << std::endl;
	std::cout << "==========================================+========================+=======================" << std::endl;
	std::cout << "|   0  NVIDIA GeForce GTX 1080      WDDM  |   00000000:26:00.0  On |                  N/A |" << std::endl;
	std::cout << "| 28%   37C    P8             11W /  180W |     701MiB /   8192MiB |      0%      Default |" << std::endl;
	std::cout << "+-----------------------------------------+------------------------+----------------------+" << std::endl;
}

void DummyProcessLayout::displayProcessList(const std::vector<GPUProcess>& processes) const
{
	std::cout << "+-----------------------------------------------------------------------------------------+" << std::endl;
	std::cout << "| Processes:                                                                              |" << std::endl;
	std::cout << "|  GPU   GI   CI        PID   Type   Process name                              GPU Memory |" << std::endl;
	std::cout << "|        ID   ID                                                               Usage      |" << std::endl;
	std::cout << "|=========================================================================================|" << std::endl;

	for (const auto& process : processes) {
		std::cout << "|    " << process.gpu << "   " << std::setw(3) << process.giid << "  " << std::setw(3) << process.ciid << "      "
			<< std::setw(4) << process.pid
			<< "    " << std::setw(3) << process.type
			<< "   " << std::left << std::setw(38) << process.processName   // Add std::left here for left alignment
			<< "      " << std::right << std::setw(3) << process.memoryUsage  // Return to std::right for numbers
			<< "      |" << std::endl;
	}

	std::cout << "+-----------------------------------------------------------------------------------------+" << std::endl;
}

void DummyProcessLayout::displayCurrentDateTimes() const
{
	// Format:
	// Day Month Date HH:MM:SS Year
	// Example: Mon Jan 1 00:00:00 2021

	// Get the current time
	time_t now = time(0);
	tm localTime;
	localtime_s(&localTime, &now);

	// Convert the time to a string
	char buffer[80];
	strftime(buffer, 80, "%a %b %d %H:%M:%S %Y", &localTime);

	// Display the current date and time
	std::cout << buffer << std::endl;
}