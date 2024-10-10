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
#include <limits>
#include <iomanip>
#include <fstream>


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
                cout << " <timestamp> " << " Core:  " << this->coreID << " " << logs[i] << endl;
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
            if (worker.joinable()) {
                worker.detach();
            }
    }
};


void displayProcesses(const map<string, Process*>& processes){
    cout << std::setfill('-') << std::setw(40) << "-" << std::endl;
    cout << "\nRunning Processes:\n";
    bool any_ongoing = false;

    cout << std::setfill(' ') << std::setw(0);
    for (const auto& pair : processes) {
        if (!pair.second->getProcessStatus()) {
            // Truncate the process name to 10 characters
            string process_name = pair.second->getName();
            int coreID = pair.second->getCoreID();
            int log_size = pair.second->getLog().size();

            if (process_name.length() > 10) {
                process_name = "..." + process_name.substr(process_name.length() - 7);
            }

            // TODO: Add timestamp
            cout << std::left   << std::setw(11) << process_name << "   "
                                << std::setw(23) << "timestamp" << "     "
                                << std::setw(10) << "Core: " << coreID << "     "
                                << std::setw(10) << log_size << "/200" << endl;
            
            any_ongoing = true;
        }
    }

    if (!any_ongoing) {
        cout << "No running processes.\n";
    }

    //TODO: Fix format/template
    cout << "\nFinished Processes:\n";
    bool any_finished = false;

    cout << std::setfill(' ') << std::setw(0);
    for (const auto& pair : processes) {
        if (pair.second->getProcessStatus()) {
            // Truncate the process name to 10 characters
            string process_name = pair.second->getName();
            int log_size = pair.second->getLog().size();

            if (process_name.length() > 10) {
                process_name = "..." + process_name.substr(process_name.length() - 7);
            }

            cout << std::left   << std::setw(11) << process_name << "   "
                                << std::setw(23) << "timestamp" << "     "
                                << std::setw(10) << "Finished" << "    "
                                << std::setw(10) << log_size << " / 200" << endl;

            any_finished = true;
        }
    }
    if (!any_finished) {
        cout << "No finished processes.\n";
    }
    cout << endl;
    cout << std::setfill('-') << std::setw(40) << "-" << std::endl;
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



int main(){
    Scheduler* scheduler = new Scheduler(4);
    map<string, Process*> processes;
    Scheduler* random = new Scheduler(1);       // placeholder only

    bool running =  true;

    displayMenu();

    while (running){
        string command;
        cout << "Enter command (screen -s <name> or screen -r <name>): ";
        getline(cin, command);


        istringstream iss(command);
        string action, option, name;
        iss >> action >> option >> name;

        if (command == "quit"){
            return 0;
        }
         else if (action == "screen" && option == "-s") {    
            string process_name = name;

            if (name.empty()) {
                cout << "No name provided for the process." << endl;
                continue;
            }

            // Schedule screen creation
            Process* process = new Process(name);
            scheduler->schedule_task(create_task, process, processes, 0, random);
            // scheduler.schedule_task(create_task, process, processes, 0, random);

            process->enterProcess();
            waitForExit();
            displayMenu();

        } else if (action == "screen" && option == "-r") {
            if (processes.count(name) > 0) {

                processes[name]->enterProcess();
                waitForExit();
                displayMenu();

            } else {
                cout << "Screen " << name << " not found." << endl;
            }
        } else if (action == "screen" && option == "bulk") {
            // TODO: Bulk creation of 10 processes (for demo)

            for (int i = 0; i < 10; i++) {
                string process_name = "Process" + to_string(i);
                cout << "Creating process " << process_name << endl;

                // Create process
                Process* process = new Process(process_name);
                scheduler->schedule_task(create_task, process, processes, 0, random);
                this_thread::sleep_for(chrono::milliseconds(1000));  // delay to capture small changes in print
            }
        } else if (action == "screen" && option == "-ls") {
            // Display processes
            displayProcesses(processes);

            // TODO: Print (export to text file)
            cout << "Enter 'print' to export logs to text file." << endl;
            string inputPrint;
            getline(cin, inputPrint);

            if (inputPrint == "print") {
                string file_name = "csopesy-log.txt";   // File name, to be created in the same directory if it doesn't exist\

                ofstream file(file_name);

                if (file.is_open()) {
                    for (const auto& pair : processes) {
                        file << "Process Name: " << pair.second->getName() << endl;
                        file << "Logs: " << endl;

                        for (int i = 0; i < pair.second->getLog().size(); i++) {
                            file << pair.second->getLog()[i] << "   # " << i << endl;
                        }
                    }

                    file.close();
                    cout << "Logs exported to " << file_name << endl;
                } else {
                    cout << "Unable to open file." << endl;
                }
            }

        } else {
            cout << "Invalid command." << endl;
        }
    }

    for (auto& pair : processes) {
        delete pair.second; // Free allocated memory for Process objects
    }

    delete scheduler;

    return 0;
}