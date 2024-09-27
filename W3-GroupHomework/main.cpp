#include "ScreenSession.h"
#include <ctime>
#include <string>
#include <vector>
#include <sstream> 
#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <limits>

/********** PROTOTYPE FUNCTIONS **********/
void displayMenu();
void displayActiveSessions();
void getCommand(std::string* command);
bool checkCommandValidity(std::string& input);
void waitForExit();
std::vector<std::string> parseCommand(const std::string& command);
/********** PROTOTYPE FUNCTIONS **********/

std::vector<ScreenSession> sessions;
/********** CONTROLLER **********/
int main(void) {

    //show main menu with instructions
    displayMenu();

    bool running = true;
    while (running) {
        //variables
        std::string command;
        bool commandIsValid = false;
        std::vector<std::string> parsedCommand;

        //get user input to select command
        getCommand(&command);

		// parse the command
        parsedCommand = parseCommand(command);

		//check if command is valid
		commandIsValid = checkCommandValidity(parsedCommand[0]);

        if (commandIsValid) {
            if (parsedCommand[0] == "initialize" || parsedCommand[0] == "scheduler-test" || parsedCommand[0] == "scheduler-stop" || parsedCommand[0] == "report-util") {
				std::cout << parsedCommand[0] << "command recognized. Doing something.\n";
				continue;
            }
            else if (parsedCommand[0] == "clear") {
                system("cls");
                displayMenu();
                continue;
            }
            else if (parsedCommand[0] == "screen") {

                if (parsedCommand[1] == "-s") {
                    std::cout << command << std::endl;


                    if (parsedCommand.size() < 3) { // Ensure there is process name
                        continue;
                    }

                    //Initialize new session class in a vector

                    //Get current timestamp
                    std::time_t currentTime = std::time(nullptr);
                    std::string processName = parsedCommand[2];
                    ScreenSession screenSession(processName, currentTime);
                    sessions.push_back(ScreenSession(processName, currentTime));

                    //Use the newly initialized session class
                    sessions.back().viewSession();
                    waitForExit();
                    displayMenu();

                }
                else if (parsedCommand[1] == "-ls") {

                    displayActiveSessions();
                    waitForExit();
                    displayMenu();

                }
                else if (parsedCommand[0] == "screen" && parsedCommand[1] == "-r") {

                    // if <name> does not exist, skip this section and user should be informed

                    // //
                    // std::string processName;
                    // std::string dumpName = "test1"; //TODO: REPLACE THIS WITH PROPER VARIABLE

                    // for (ScreenSession &session : sessions) {
                    //     processName = session.getProcessName();
                    //     if (processName == dumpName) {
                    //         session.viewSession();
                    //         break;
                    //     }
                    // }
                    // //

                    if (parsedCommand.size() < 3) {
                        std::cout << "\n";
                        std::cout << "Error: No process name provided." << std::endl;
                        std::cout << "Please type 'exit' to go back to the menu." << std::endl;
                        std::cout << "\n";
                    }
                    else {
                        std::string processName = parsedCommand[2];
                        bool sessionFound = false;

                        for (ScreenSession& session : sessions) {
                            if (session.getProcessName() == processName) {
                                session.viewSession();
                                sessionFound = true;
                                break;
                            }
                        }

                        if (!sessionFound) {
                            std::cout << "\n";
                            std::cout << "Error: Session with name '" << processName << "' does not exist." << std::endl;
                            std::cout << "Please type 'exit' to go back to the menu." << std::endl;
                            std::cout << "\n";
                        }
                    }

                    waitForExit();
                    displayMenu();
                }
                else {
                    std::cout << "Unknown command: " << parsedCommand[0] << " " << parsedCommand[1] << ". I do not understand.\n";
                }
            }
            else if (parsedCommand[0] == "exit") {
                return 0;
            }
            else {
                std::cout << "Unknown command: " << parsedCommand[0] << ". I do not understand.\n";
            }
        }
    }
}
/********** CONTROLLER **********/


/********** SHOW MAIN MENU **********/
void displayMenu() {
    std::cout << "  ____    ____      ___     ____    _______    ____    __   __		\n";
    std::cout << " / ___|  / ___|    / _ \\   |  _ \\   |  ___|   / ___|   \\ \\ / /	\n";
    std::cout << "| |      \\___ \\   | | | |  | |_) |  |  __|    \\___ \\    \\ V /	\n";
    std::cout << "| |___    ___) |  | |_| |  |  __/   | |___     ___) |    | |			\n";
    std::cout << " \\____|  |____/    \\___/   |_|      |_____|   |____/     |_|		\n";

    HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_color, 10);
    std::cout << "Hello, Welcome to CSOPESY commandline!\n";
    SetConsoleTextAttribute(console_color, 14);
    std::cout << "Type 'exit' to quit, 'clear' to clear the screen\n";
    SetConsoleTextAttribute(console_color, 15);
}
/********** SHOW MAIN MENU **********/


std::vector<std::string> parseCommand(const std::string& command) {
    std::istringstream iss(command);
    std::vector<std::string> words;
    std::string word;

    while (iss >> word && words.size() < 3) {
        words.push_back(word);  // Add only the first two words
    }

    return words; // vector with 2 words
}

void displayActiveSessions() {
    system("cls");
    std::cout << "All sessions\n";
    for (ScreenSession& session : sessions) {
        session.viewSessionSummary();  // Call the method to display each session

    }
    std::cout << std::endl;
}

void getCommand(std::string* command) {
	std::cout << "Enter command: ";
    std::getline(std::cin, *command);
}

void waitForExit() {
    bool running = true;

    while (running) {
        std::string input;
        std::cin >> input;
        if (input == "exit") {
            running = false;
        }
    }
    system("cls");

}


bool checkCommandValidity(std::string& input) {
    bool isValid = false;
    //TODO: update the command names to have spaces once spaced version is working
    std::string commands[] = { "initialize", "scheduler-test", "scheduler-stop", "report-util", "clear", "exit",
                                "screen" };

    for (std::string& command : commands) {
        if (command == input) {
            /*if (command != "clear" && command != "exit" && command != "screen") {
                std::cout << command << " command recognized. Doing something.\n";
            }*/

            isValid = true;
            break;
        }
    }
    return isValid;
}
