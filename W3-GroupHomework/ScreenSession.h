#include <iostream>
#include <string>
#include <ctime>

class ScreenSession {
    std::string processName;
    std::string timestamp;

    public:
        ScreenSession(std::string &processName, std::string timestamp) {
            this->processName = processName;
            this->timestamp = timestamp;
        }

    void viewSession() {
        system("cls");
        std::cout << "Process Name: " << this->processName << "          Timestamp: " << this->timestamp << std::endl;
        std::cout << "#1" << std::endl;
        std::cout << "#2" << std::endl;
        std::cout << "#3" << std::endl;
        std::cout << "#4" << std::endl;
        std::cout << "#5" << std::endl;
    }

    void viewSessionSummary() {
        std::cout << "Process Name: " << this->processName << "          Timestamp: " << this->timestamp << std::endl;
    }

    std::string getProcessName() {
        return this->processName;
    }

    bool leaveSession() {
        return false;
    }
};