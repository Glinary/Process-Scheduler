#include "ScreenSession.h"
#include <ctime>
#include <string>
#include <vector>
#include <sstream> 
#include <iostream>
#include <iomanip>
#include <limits>

/********** PROTOTYPE FUNCTIONS **********/
void displayMenu(); 
void displayActiveSessions();
void getCommand(std::string *command);
bool checkCommandValidity(std::string &input);
void waitForExit();
std::vector<std::string> parseCommand(const std::string& command);
void clearNewline();
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
        bool commandIsValid;
        std::vector<std::string> parsedCommand;

        //get user input to select command
        getCommand(&command);
        parsedCommand = parseCommand(command);
        // std::cout << command << std::endl;

        if (parsedCommand[0]== "quit") {

            return 0;

        } else if (parsedCommand[0] == "screen") {

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
                sessions.push_back(ScreenSession (processName, currentTime));

                //Use the newly initialized session class
                sessions.back().viewSession();
                waitForExit();
                displayMenu();

            }  else if (parsedCommand[1] == "-ls") {

                displayActiveSessions();
                waitForExit();
                displayMenu();
            
            } else if (parsedCommand[0] == "screen" && parsedCommand[1] == "-r") {

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
                } else {
                    std::string processName = parsedCommand[2];
                    bool sessionFound = false;

                    for (ScreenSession &session : sessions) {
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
            } else {
                std::cout << "I do not understand";
            }
        } else {
            std::cout << "I do not understand";
        }

        clearNewline();
    }

        
}
/********** CONTROLLER **********/


/********** SHOW MAIN MENU **********/ 
void displayMenu() {
    std::cout << "\n";
    std::cout << "Main Menu" << std::endl;
    std::cout << "\n";
    std::cout << std::left << std::setw(20) << "command:" << "description" << std::endl;
    std::cout << std::left << std::setw(20) << "screen-s<name>" << "creates a new screen session" << std::endl;
    std::cout << std::left << std::setw(20) << "screen-ls" << "displays all active screen sessions" << std::endl;
    std::cout << std::left << std::setw(20) << "screen -r <name>" << "redraws an existing screen session" << std::endl;
    std::cout << std::left << std::setw(20) << "quit" << "quit program" << std::endl;
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
    for (ScreenSession &session : sessions) {
        session.viewSessionSummary();  // Call the method to display each session
        
    }
    std::cout << std::endl;
}

void getCommand(std::string *command) {
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

void clearNewline(){
    // Ignore the leftover newline character
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

}

bool checkCommandValidity(std::string &input) {
    bool isValid = false;
    //TODO: update the command names to have spaces once spaced version is working
    std::string commands[] = {"screen-s<name>", "screen-ls", "screen-r<name>"};

    for (std::string &command : commands) {
        if (command == input) {
            isValid = true;
            break;
        }
    }
    return isValid;
}


