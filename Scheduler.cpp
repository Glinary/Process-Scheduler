#include "Scheduler.h"


Scheduler* Scheduler::scheduler = nullptr;				// Initialize the instance of ConsoleManager

Scheduler* Scheduler::getInstance() {
    return scheduler;
}

void Scheduler::initialize(){
	scheduler =  new Scheduler();
}

Scheduler::Scheduler() 
    : isShuttingDown(false) {} // Initialize the shutdown flag

// Initialize threads based on `maxcpu`
void Scheduler::setCore(int maxcpu) {
    maxCores = maxcpu; // Store the maximum cores
    threadPool = std::make_unique<ThreadPool>(maxCores);

	std::cout << "Finished initializing core..." << std::endl;
}

// Schedule a process
void Scheduler::scheduleProcess(const std::shared_ptr<Process>& process) {
   std::cout << "Scheduling process..." << std::endl;
    threadPool->enqueue([process]() {
        std::cout << "Running process in thread." << std::endl;
        process->initProcess();  // Run the process's initialization
    });
}

