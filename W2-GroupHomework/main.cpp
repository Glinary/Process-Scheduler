#include "ScreenSession.h"
#include "commandManager.h"
#include <ctime>
#include <string>
#include <vector>

/********** PROTOTYPE FUNCTIONS **********/
void displayMenu(); 
/********** PROTOTYPE FUNCTIONS **********/

std::vector<ScreenSession> sessions;
/********** CONTROLLER **********/
int main(void) {

    

    //show main menu with instructions
    displayMenu();

    bool running = true;
    while (running) {
        //get user input to select command

        //draw console

        //check if system should keep running

        //Get current timestamp
        std::time_t currentTime = std::time(nullptr);
        std::string processName = "test1";
        //Initialize new session class in a vector
        ScreenSession screenSession(processName, currentTime);
        sessions.push_back(ScreenSession (processName, currentTime));
        //Use the newly initialized session class
        sessions.back().viewSession();
        //Leave new session
        running = sessions.back().leaveSession();

    }

    

    return 0;
}
/********** CONTROLLER **********/


/********** SHOW MAIN MENU **********/ 
void displayMenu() {
    std::cout << "Main Menu" << std::endl;
    std::cout << "Commands" << std::endl;
}
/********** SHOW MAIN MENU **********/ 



