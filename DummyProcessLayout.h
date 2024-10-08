#pragma once
#include "TypedefRepo.h"
#include "AConsole.h"
#include "Process.h"
#include <memory>
#include <vector>

// Define the GPUProcess structure to hold dummy data
struct GPUProcess {
    int gpu;
    String giid;
    String ciid;
	int pid;
    String type;
    String processName;
    String memoryUsage;
};

class DummyProcessLayout : public AConsole
{
public:
    DummyProcessLayout(std::shared_ptr<Process> process, String processName);

    // Override the pure virtual functions of AConsole
    void onEnabled() override;
    void display() override;
    void process() override;

private:
    std::shared_ptr<Process> attachedProcess;		// The process attached to the screen; class the represents the process; for every base screen, there is a process that contains another set of commands
    bool validateCommand(String& input) const;      // Validates the command
    void displayGPUSummaries() const;               // Displays the GPU summaries
    void displayProcessList(const std::vector<GPUProcess>& processes) const;  // Displays the process list
    void displayCurrentDateTimes() const;           // Displays the current date and time
};
