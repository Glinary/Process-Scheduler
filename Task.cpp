#include "Task.h"
#include "TypedefRepo.h"
#include "Process.h" // Ensure this header can be found
#include <iostream>
#include <format>



// Constructor implementation
Task::Task(std::shared_ptr<Process> process) : storedProcess(process) {}

// Getter method implementation
std::shared_ptr<Process> Task::getProcess() const {
    return storedProcess;
}

void Task::initProcess() const {
    storedProcess->initProcess();
}
