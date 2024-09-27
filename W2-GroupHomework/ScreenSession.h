#include <iostream>
#include <string>
#include <ctime>

class ScreenSession {
    std::string processName;
    std::time_t timestamp;

    public:
        ScreenSession(std::string &processName, std::time_t timestamp) {
            this->processName = processName;
            this->timestamp = timestamp;
        }

    void viewSession() {
        std::cout << "Process Name: " << processName << std::endl;
    }

    bool leaveSession() {
        return false;
    }
};