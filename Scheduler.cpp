#include "Scheduler.h"

Scheduler* Scheduler::sched = nullptr;				// Initialize the instance of Scheduler

Scheduler* Scheduler::getInstance() {
    return sched;
}

void Scheduler::initialize(const MainConsole::Config& config) {
    sched = new Scheduler(config); 
}

Scheduler::Scheduler(const MainConsole::Config& config)
    : isShuttingDown(false), config(config), isBatchProcess(false), nextKey(0)
{
    // Launch a detached thread to handle scheduling independently
    std::thread([this, config = this->config, cycleCounter = this->cycleCounter]() mutable {
        ThreadPool::initialize(config.num_cpu, config.scheduler, config.quantum_cycles);
        int batchProcessCount = 1;

        while (!isShuttingDown) {
            // std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Uncomment if delay is needed

            if (cycleCounter % config.batch_process_freq == 0 && isBatchProcess) {
                generateBatchProcess(config, batchProcessCount); 
                batchProcessCount++;
            }

            cycleCounter++;
        }
    }).detach(); // Detach the thread to run independently
}

void Scheduler::generateBatchProcess(const MainConsole::Config& config, int batchProcessCount) {
    String processName = "Process" + std::to_string(batchProcessCount);
    std::shared_ptr<Process> newProcess = std::make_shared<Process>(processName, config);
    std::shared_ptr<BaseScreen> newScreen = std::make_shared<BaseScreen>(newProcess, processName);	// Create a new screen

    scheduleProcess(newProcess);

    ConsoleManager::getInstance()->addProcesses(newProcess);
    ConsoleManager::getInstance()->registerScreen(newScreen);
}

void Scheduler::setBatch(bool status){
    this->isBatchProcess = status;
}

// Schedule a process
void Scheduler::scheduleProcess(const std::shared_ptr<Process>& process) {
    int key = ThreadPool::getInstance()->getNextKey();
    ThreadPool::getInstance()->enqueue([process]() {
        process->initProcess();  // Run the process's initialization
    });
}
