#include "MainConsole.h"
#include "TypedefRepo.h"
#include "ConsoleManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>  // for std::quoted
#include <algorithm> // for transform() - converting string to lowercase
#include <Windows.h>
#include "DummyProcessLayout.h"


// Constructor: Set the name of the console when MainConsole is instantiated
MainConsole::MainConsole() : AConsole("MainConsole") {}

// Override of onEnabled: This is called when the screen is shown for the first time
void MainConsole::onEnabled() {
	ASCIITextHeader();
}

// Override of display: Called to draw the screen each frame
void MainConsole::display() {
	// ASCIITextHeader();
}

// Override of process: Handle input commands or other processes here
void MainConsole::process() {
	String commandMain;
	bool isFirstCommand = true;
	bool isValidCommand;

	while (true) {
		std::cout << "Enter a command: ";
		std::getline(std::cin, commandMain);

		// Check if the first command is valid
		if (isFirstCommand) {
			isValidCommand = isValidFirstCommand(commandMain);
			
			if (!isValidCommand) {
				std::cerr << "Error: Please initialize the program first. Type command \"initialize\"\n" << std::endl;
				continue; // Re-prompt for the command without displaying the header
			}
			else {
				if (commandMain == "initialize") {
					std::cout << "Initializing the program...\n" << std::endl;
					String filename = "config.txt";

					if (!loadConfigFile(filename)) {
						std::cerr << "Error: Unable to load config.txt. Please try again." << std::endl;
					}
					else {
						std::cout << "Program initialized." << std::endl;
						std::cout << "Number of CPUs: " << static_cast<int>(config.num_cpu) << std::endl;
						std::cout << "Scheduler: " << config.scheduler << std::endl;
						std::cout << "Quantum Cycles: " << config.quantum_cycles << std::endl;
						std::cout << "Batch Process Frequency: " << config.batch_process_freq << std::endl;
						std::cout << "Minimum Instructions: " << config.min_ins << std::endl;
						std::cout << "Maximum Instructions: " << config.max_ins << std::endl;
						std::cout << "Delays per Execution: " << config.delays_per_exec << std::endl;
						std::cout << "______________________________________________________________\n";

						isFirstCommand = false;
					}
					
					continue;
				}
				else if (commandMain == "exit") {
					ConsoleManager::getInstance()->exitApplication();
					exit(0);
				}
			}
		}	// If first command end
		
		// Process the commands after the first command or initializing the program
		if (!isFirstCommand) {
			isValidCommand = validateCommand(commandMain);

			if (!isValidCommand) {
				if (commandMain.substr(0, 9) == "screen -s" && commandMain.length() <= 9) {
					std::cerr << "Error: No process name provided. Please try again." << std::endl;
				}
				else if (commandMain.substr(0, 9) == "screen -r" && commandMain.length() <= 9) {
					/*std::cerr << "Error: You are in Main Screen. There is no previous screen. Please try again." << std::endl;*/
					std::cerr << "Error: No process name provided. Please try again." << std::endl;
				}
				else {
					std::cout << commandMain << " command not recognized. Please try again." << std::endl;
				}
				continue; // Re-prompt for the command without displaying the header
			}

			// If the command is recognized
			if (commandMain == "initialize") {
				std::cout << "Program already initialized.\n" << std::endl;
				continue;
			}
			else if (commandMain == "exit") {
				ConsoleManager::getInstance()->exitApplication();
				exit(0);
			}
			else if (commandMain == "clear") {
				system("cls");
				onEnabled();
			}
			else if (commandMain.substr(0, 9) == "screen -s" && commandMain.length() > 9) {			// ensure there is process name
				ConsoleManager::getInstance()->exitApplication();									// Stop the main console process
				String processName = commandMain.substr(10, commandMain.length() - 10);				// get the process name

				// Create a new process and attach it to a new screen
				std::shared_ptr<Process> newProcess = std::make_shared<Process>(processName);
				std::shared_ptr<BaseScreen> newScreen = std::make_shared<BaseScreen>(newProcess, processName);	// Create a new screen

				// Register the new screen and switch to it
				ConsoleManager::getInstance()->registerScreen(newScreen);					// Register the new screen
				ConsoleManager::getInstance()->switchToScreen(processName);					// Switch to the new screen	
				ConsoleManager::getInstance()->process();									// Process the new screen
				ConsoleManager::getInstance()->drawConsole();								// Draw the new screen
			}
			else if (commandMain.substr(0, 9) == "screen -r") {
				ConsoleManager::getInstance()->exitApplication();					// Stop the main console process
				String processName = commandMain.substr(10);						// Get the process name

				ConsoleManager::getInstance()->switchToScreen(processName);			// Switch to the previous screen
				ConsoleManager::getInstance()->process();							// Process the previous screen
				ConsoleManager::getInstance()->drawConsole();						// Draw the previous screen
			}
			else if (commandMain == "screen -ls") {
				// List all the screens
				std::cout << "List of screens: " << std::endl;
				ConsoleManager::getInstance()->printScreenNames();



				/*for (auto const& screen : ConsoleManager::getInstance()->consoleTable) {
					std::cout << screen.first << std::endl;
				}*/
			}
			else if (commandMain == "dummy-layout") {
				// clear the screen
				system("cls");

				// display the dummy layout
				DummyProcessLayout dummyLayout;
				dummyLayout.onEnabled();
			}
			else {
				recognizeCommand(commandMain);
				continue;
			}
		}	// If not first command end
	}	// Main loop end
}	// Process end


void MainConsole::ASCIITextHeader() const {
	std::cout << "  ____    ____      ___     ____    _______    ____    __   __		\n";
	std::cout << " / ___|  / ___|    / _ \\   |  _ \\   |  ___|   / ___|   \\ \\ / /	\n";
	std::cout << "| |      \\___ \\   | | | |  | |_) |  |  __|    \\___ \\    \\ V /	\n";
	std::cout << "| |___    ___) |  | |_| |  |  __/   | |___     ___) |    | |			\n";
	std::cout << " \\____|  |____/    \\___/   |_|      |_____|   |____/     |_|		\n";

	std::cout << "______________________________________________________________\n";

	HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_color, 10);
	std::cout << "Welcome to CSOPESY Emulator!\n";
	std::cout << "\n";

	displayDevelopers();
	std::cout << "______________________________________________________________\n";

	SetConsoleTextAttribute(console_color, 14);
	std::cout << "Type 'exit' to quit, 'clear' to clear the screen\n";
	SetConsoleTextAttribute(console_color, 15);
}


void MainConsole::displayDevelopers() const {
	HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_color, 15);

	std::cout << "Developers: \n";
	std::cout << "1. Abenoja, Amelia Joyce L. \n";
	std::cout << "2. Cuales, Bianca Mari A. \n";
	std::cout << "3. Culala, Mary Erika L. \n";
	std::cout << "4. Uy,Gleezell Vina A. \n";
	std::cout << "\n";
	std::cout << "Last Updated: 10-24-2024\n";
}

bool MainConsole::isValidFirstCommand(String command) const {
	bool isValid = false;

	String commandList[] = { "initialize", "exit" };

	// Check if the first word of the input is a valid first command
	String inputCommand = command.substr(0, command.find(" "));
	std::transform(inputCommand.begin(), inputCommand.end(), inputCommand.begin(), ::tolower);

	for (String command : commandList) {
		if (inputCommand == command) {
			isValid = true;
			break;
		}
	}

	return isValid;
}


bool MainConsole::validateCommand(String& input) const {
	bool isValid = false;

	String commandList[] = { "initialize", "exit", "clear",
							"scheduler-test", "scheduler-stop", "report-util",
							"screen",
							"dummy-layout" };

	// Check if the first word of the input is a valid command
	String inputCommand = input.substr(0, input.find(" "));
	std::transform(inputCommand.begin(), inputCommand.end(), inputCommand.begin(), ::tolower);

	for (String command : commandList) {
		if (inputCommand == command) {
			if (command == "screen") {
				if (isValidScreenCommand(input)) {
					isValid = true;
					break;
				}
			}
			else {
				isValid = true;
				break;
			}
		}
	}


	return isValid;
}

bool MainConsole::isValidScreenCommand(String command) const {
	bool isValid = false;

	String screenCommandList[] = { "screen -s", "screen -ls", "screen -r" };

	for (String screenCommand : screenCommandList) {
		if (command.substr(0, 9) == screenCommand) {
			// Check if the command has process name after the screen command
			if (command.length() > 9) {
				isValid = true;
				break;
			}
		}
		else if (command.substr(0, 10) == screenCommand) {
			isValid = true;
			break;
		}
	}

	return isValid;
}


void MainConsole::recognizeCommand(String command) const {
	std::cout << command << " command recognized. Doing something...\n";
}

bool MainConsole::loadConfigFile(String& filename) {
	bool isLoaded = false;

	try {
		// read the config.txt
		std::ifstream configFile(filename);
		if (!configFile.is_open()) {
			throw std::runtime_error("Error: config.txt not found.");
		}
		else {
			std::string line;
			while (std::getline(configFile, line)) {
				//std::cout << line << std::endl;
				if (!parseConfigFile(line)) {
					std::cerr << "Error: Invalid configuration file." << std::endl;
					configFile.close();
					break;
				}
			}

			configFile.close();
			isLoaded = true;
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return isLoaded;
}

bool MainConsole::parseConfigFile(String& line) {
	std::istringstream iss(line);
	String key;

	if (!(iss >> key)) {
		return false; // Empty line
	}

	try {
		if (key == "num-cpu") {
			int value;
			if (iss >> value && value >= 1 && value <= 128) {
				config.num_cpu = static_cast<uint8_t>(value);
			}
			else {
				throw std::invalid_argument("num-cpu must be between 1 and 128");
			}
		}
		else if (key == "scheduler") {
			String value;
			iss >> std::quoted(value); // Handles quoted strings
			if (value == "rr" || value == "fcfs") {
				config.scheduler = value;
			}
			else {
				throw std::invalid_argument("scheduler must be \"rr\" or \"fcfs\"");
			}
		}
		else if (key == "quantum-cycles") {
			uint32_t value;
			if (iss >> value && value >= 1) {
				config.quantum_cycles = value;
			}
			else {
				throw std::invalid_argument("quantum-cycles must be >= 1");
			}
		}
		else if (key == "batch-process-freq") {
			uint32_t value;
			if (iss >> value && value >= 1) {
				config.batch_process_freq = value;
			}
			else {
				throw std::invalid_argument("batch-process-freq must be >= 1");
			}
		}
		else if (key == "min-ins") {
			uint32_t value;
			if (iss >> value && value >= 1) {
				config.min_ins = value;
			}
			else {
				throw std::invalid_argument("min-ins must be >= 1");
			}
		}
		else if (key == "max-ins") {
			uint32_t value;
			if (iss >> value && value >= config.min_ins) {
				config.max_ins = value;
			}
			else {
				throw std::invalid_argument("max-ins must be >= min-ins");
			}
		}
		else if (key == "delay-per-exec") {
			uint32_t value;
			if (iss >> value) {
				config.delays_per_exec = value;
			}
			else {
				throw std::invalid_argument("Invalid delay-per-exec value");
			}
		}
		else {
			throw std::invalid_argument("Unknown configuration key: " + key);
		}
	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
		return false;
	}

	return true;
}

MainConsole::Config MainConsole::getConfig() const {
	return config;
}