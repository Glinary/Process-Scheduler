#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <queue>
#include <functional>
#include <map>
#include <sstream>
#include <condition_variable>
#include <iomanip>
#include <limits>
#include <ctime>
#include <iostream>

using namespace std;

class Process {

    private:
        const string name;
        vector<string> logs;
        bool finished = false;
        int coreID;
        // mutex mutex_;

    public:
        explicit Process(const string name) : name(name){}

        void enterProcess() {
            system("cls");
            cout << "Process Name: " << this->name << endl;
            cout << "Logs: " << endl;

            // TODO: Fix template: <Timestamp  Core#  Print Message>
            for (int i = 0; i < logs.size(); i++){
                  cout << logs[i] << "   # " << i << endl; 
            }
        }

        void addLog(string log){
            this->logs.push_back(log);
        }  
        vector<string> getLog(){
            return this->logs;
        }
        string getName(){
            return this->name;
        }
        void setProcessStatus(bool status){
            this->finished = status;
        }

        bool getProcessStatus(){
            return this->finished;
        }
        void setCoreID(int coreID){
            this->coreID = coreID;
        }
        int getCoreID(){
            return this->coreID;
        }
};

class Scheduler {

    private:
        int max_threads;
        vector<thread> workers;
        queue<function<void()>> task_queue;
        condition_variable condition;
        mutex queue_mutex;
        mutex core_mutex;
        bool stop = false;
        vector<bool> coresStatus;

    public:

    // constructor - at creation of scheduler create 4 threads. 
     Scheduler(int max_threads) : max_threads(max_threads), coresStatus(max_threads, false) {
        for (size_t i = 0; i < max_threads; ++i) {
            workers.emplace_back([this, i] {
                while (true) {
                    function<void()> task;
                    unique_lock<mutex> lock(queue_mutex);
                   if (!task_queue.empty()) {
                        task = move(task_queue.front());
                        task_queue.pop();
                        lock.unlock(); // Unlock the mutex before executing the task
                        task();
                    } 
                }
            });
        }
    }

    
    int getAvailableCore(){
         lock_guard<mutex> lock(core_mutex);

         for (int i = 0; i < max_threads; ++i) {
            if (!coresStatus[i]) {  
                coresStatus[i] = true;  // Mark it as busy
                return i;  
            }
        }
        return -1;  // All cores are busy
    }

    void schedule_task(void (*func)(Process*,  map<string, Process*>&, int, Scheduler*), Process* process,  map<string, Process*>& processes, int coreID, Scheduler* sched) {
        
        {
            lock_guard<mutex> lock(queue_mutex);
            task_queue.push([func, process, &processes, coreID, this]() {
                func(process, processes, coreID, this);
            });
        }

        condition.notify_one();
    };

     // Release the core when the process finishes
    void release_core(int coreID) {
        lock_guard<mutex> lock(core_mutex);
        coresStatus[coreID] = false;  // Mark the core as available again
    }

    // Stop the pool and join all threads
    ~Scheduler() {
        {
            lock_guard<mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (thread &worker : workers)
            worker.join();
    }
};


void displayProcesses(const map<string, Process*>& processes){
    cout << "\nRunning Processes:\n";
    bool any_ongoing = false;

    for (const auto& pair : processes) {
        if (!pair.second->getProcessStatus()) {
            // TODO: Add timestamp
            cout << pair.second->getName() << "timestamp" << ", Core: " << pair.second->getCoreID() << " " << pair.second->getLog().size() << "/200" << endl;
            any_ongoing = true;
        }
    }

    if (!any_ongoing) {
        cout << "No running processes.\n";
    }

    //TODO: Fix format/template
    cout << "\nFinished Processes:\n";
    bool any_finished = false;
    for (const auto& pair : processes) {
        if (pair.second->getProcessStatus()) {
            cout << "Process: " << pair.second->getName() << ", Messages: " << pair.second->getLog().size() << endl;
            any_finished = true;
        }
    }
    if (!any_finished) {
        cout << "No finished processes.\n";
    }
    cout << endl;

}

// task that gets added to queue (function tasks)
void create_task(Process* process, map<string, Process*>& processes, int coreID, Scheduler* sched){
    processes[process->getName()] =  process;

    coreID = sched-> getAvailableCore();
    process->setCoreID(coreID);
    
    for (int i = 0; i < 200; ++i) {
        process->addLog("message now");
        this_thread::sleep_for(chrono::milliseconds(100));  // delay to capture small changes in print
    }

    process->setProcessStatus(true);    // set process to finished
    sched->release_core(coreID);        // release core when finished

};


void displayMenu() {
    std::cout << "\n";
    std::cout << "Main Menu" << std::endl;
    std::cout << "\n";
    std::cout << std::left << std::setw(20) << "command:" << "description" << std::endl;
    std::cout << std::left << std::setw(20) << "screen -s <name>" << "creates a new screen session" << std::endl;
    std::cout << std::left << std::setw(20) << "screen -ls" << "displays all active screen sessions" << std::endl;
    std::cout << std::left << std::setw(20) << "screen -r <name>" << "redraws an existing screen session" << std::endl;
    std::cout << std::left << std::setw(20) << "quit" << "quit program" << std::endl;
}

void waitForExit() {
    bool running = true;

    while (running) {
        string input;
        getline(cin, input);
        if (input == "exit") {
            running = false;
        }
    }
    system("cls");
    
}

std::vector<std::string> parseCommand(const std::string& command) {
    std::istringstream iss(command);
    std::vector<std::string> words;
    std::string word;

    while (iss >> word && words.size() < 3) {
        words.push_back(word);  // Add only the first two words
    }

    return words; // vector with 2 words
}


void getCommand(string *command) {
    getline(cin, *command);
}

void clearNewline(){
    // Ignore the leftover newline character
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

}

// int main(){
//     Scheduler scheduler(4);
//     map<string, Process*> processes;
//     Scheduler* random = new Scheduler(1);       // placeholder only
//     bool running =  true;

//     displayMenu();

//     while (running){
//         string command;
//         vector<string> parsedCommand;

//         getCommand(&command);
//         parsedCommand = parseCommand(command);
        
//         if (parsedCommand[0]== "quit"){
//             return 0;
//         }
//         else if (parsedCommand[0] == "screen") {

//             if (parsedCommand[1] == "-s") {

//                  if (parsedCommand.size() < 3) { // Ensure there is process name
//                     continue;
//                 }

//                 // Schedule process creation
//                 string processName = parsedCommand[2];
//                 Process* process = new Process(processName);
                
//                 scheduler.schedule_task(create_task, process, processes, 0, random);

//                 process->enterProcess();
//                 waitForExit();
//                 displayMenu();

//             } 
//             else if (parsedCommand[0] == "screen" && parsedCommand[1] == "-r") {
//                 string processName = parsedCommand[2];

//                 if (processes.count(processName) > 0) {

//                     processes[processName]->enterProcess();
//                     waitForExit();
//                     displayMenu();

//                 } else {
//                     cout << "Screen " << processName << " not found." << endl;
//                 }  

//             } else if (parsedCommand[1] == "-ls") {
//                 displayProcesses(processes);
            
//             // TODO: Bulk creation of 10 processes (for demo)
//             // TODO: Print (export to text file)

//             } else {
//                 cout << "Invalid command." << endl;
//             }
//         } else {
//             cout << "I do not understand" << std::endl;
//         }

//         clearNewline();
//     }

//     for (auto& pair : processes) {
//         delete pair.second; // Free allocated memory for Process objects
//     }

//     return 0;
// }


int main(void) {
     Scheduler scheduler(4);
    map<string, Process*> processes;
    Scheduler* random = new Scheduler(1);       // placeholder only

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

        std::cout << command << std::endl;
        if (parsedCommand[0]== "quit") {

            return 0;

        } else if (parsedCommand[0] == "screen") {

            if (parsedCommand[1] == "-s") {
                std::cout << command << std::endl;
            
                if (parsedCommand.size() < 3) { // Ensure there is process name
                    continue;
                }

                //Schedule process creation
                string processName = parsedCommand[2];
                Process* process = new Process(processName);
                
                scheduler.schedule_task(create_task, process, processes, 0, random);

                process->enterProcess();

                waitForExit();
                displayMenu();

            }  else if (parsedCommand[1] == "-ls") {

                displayProcesses(processes);
                waitForExit();
                displayMenu();
            
            } else if (parsedCommand[0] == "screen" && parsedCommand[1] == "-r") {

                if (parsedCommand.size() < 3) {
                    std::cout << "\n";
                    std::cout << "Error: No process name provided." << std::endl;
                    std::cout << "Please type 'exit' to go back to the menu." << std::endl;
                    std::cout << "\n";
                } else {
                    std::string processName = parsedCommand[2];

                    if (processes.count(processName) > 0) {

                    processes[processName]->enterProcess();
                    waitForExit();
                    displayMenu();

                    } else {
                        cout << "Screen " << processName << " not found." << endl;
                    }  
                        
                }
                waitForExit();
                displayMenu();
            } else {
                std::cout << "I do not understand" << std::endl;
            }
        } else {
            std::cout << "I do not understand" << std::endl;
        }

        clearNewline();
    }

        
}