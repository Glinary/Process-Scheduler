#include "ScreenSession.h"
#include <ctime>
#include <string>
#include <vector>

/********** PROTOTYPE FUNCTIONS **********/
void displayMenu(); 
void displayActiveSessions();
void getCommand(std::string *command);
bool checkCommandValidity(std::string &input);
void waitForExit();
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

        //get user input to select command
        getCommand(&command);

        if (command == "quit") {

            return 0;

        //TODO: replace this with multiple word strings e.g. "screen -s <name>"
        } else if (command == "screen-s<name>") {

            //Initialize new session class in a vector

            //Get current timestamp
            std::time_t currentTime = std::time(nullptr);
            std::string processName = "test1";
            ScreenSession screenSession(processName, currentTime);
            sessions.push_back(ScreenSession (processName, currentTime));

            //Use the newly initialized session class
            sessions.back().viewSession();
            waitForExit();
            displayMenu();

        //TODO: replace this with multiple word strings e.g. "screen -ls"
        } else if (command == "screen-ls") {

            displayActiveSessions();
            waitForExit();
            displayMenu();

        //TODO: replace this with multiple word strings e.g. "screen -r <name>"
        } else if (command == "screen-r<name>") {

            //TODO: if <name> does not exist, skip this section and user should be informed

            std::string processName;
            std::string dumpName = "test1"; //TODO: REPLACE THIS WITH PROPER VARIABLE

            for (ScreenSession &session : sessions) {
                processName = session.getProcessName();
                if (processName == dumpName) {
                    session.viewSession();
                    break;
                }
            }
            waitForExit();
            displayMenu();

        } else {
            std::cout << "I do not understand";
        }

        
    }

    

    return 0;
}
/********** CONTROLLER **********/


/********** SHOW MAIN MENU **********/ 
void displayMenu() {
    std::cout << "Main Menu" << std::endl;
    std::cout << "command:" << "          "<< "description" << std::endl; 
    //TODO: update the command names to have spaces once spaced version is working
    std::cout << "screen-s<name> " << "          " << "creates a new screen session" << std::endl;
    std::cout << "screen-ls" << "          " << "displays all active screen sessions" << std::endl;
    std::cout << "screen-r<name> " << "          " << "redraws an existing screen session" << std::endl;
    std::cout << "quit " << "          " << "quit program" << std::endl;
}
/********** SHOW MAIN MENU **********/ 

void displayActiveSessions() {
    system("cls");
    std::cout << "All sessions\n";
    for (ScreenSession &session : sessions) {
        session.viewSessionSummary();  // Call the method to display each session
        
    }
    std::cout << std::endl;
}

void getCommand(std::string *command) {
    std::cout << "" << std::endl;
    std::cin >> *command;
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


