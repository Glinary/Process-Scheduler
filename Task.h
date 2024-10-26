#pragma once
#include <memory>
#include "MainConsole.h"
#include "TypedefRepo.h"
#include "AConsole.h"
#include "Process.h"

class Task {
public:
    Task(std::shared_ptr<Process> process);
    std::shared_ptr<Process> getProcess() const { return storedProcess; } // Use the new name
    void initProcess() const;


private:
    std::shared_ptr<Process> storedProcess; // Changed variable name here
};