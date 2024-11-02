#include "Scheduler.h"


Scheduler* Scheduler::sched = nullptr;				// Initialize the instance of ConsoleManager

Scheduler* Scheduler::getInstance() {
    return sched;
}

void Scheduler::initialize(const MainConsole::Config& config) {
    sched = new Scheduler(config); 
    
}

Scheduler::Scheduler(const MainConsole::Config& config)
    : isShuttingDown(false), config(config), isBatchProcess(false)
{
    std::thread([this, config = this->config, cycleCounter = this->cycleCounter]() mutable {
        threadPool = std::make_unique<ThreadPool>(config.num_cpu);
        int batchProcessCount = 1;


        while (!isShuttingDown) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            if (cycleCounter % config.batch_process_freq == 0 && isBatchProcess) {

                if (batchProcessCount == 50){
                    isBatchProcess = false;
                }

                // std:: cout << cycleCounter << std::endl;
                generateBatchProcess(config, batchProcessCount); 
                batchProcessCount++;
            }

            cycleCounter++;

        }
    }).detach(); // Detach the thread to run independently
}

void Scheduler::generateBatchProcess(const MainConsole::Config& config, int batchProcessCount) {
        String processName = "Process" + std::to_string(batchProcessCount);
        // std:: cout << processName << std::endl;
		std::shared_ptr<Process> newProcess = std::make_shared<Process>(processName, config);
        std::shared_ptr<BaseScreen> newScreen = std::make_shared<BaseScreen>(newProcess, processName);	// Create a new screen

        scheduleProcess(newProcess);

        ConsoleManager::getInstance()->addProcesses(newProcess);
        ConsoleManager::getInstance()->registerScreen(newScreen);

    }

// setup scheduler
// void Scheduler::setupScheduler(uint8_t num_cpu, String scheduler, uint32_t quantum_cycles, uint32_t batch_process_freq, uint32_t min_ins, uint32_t max_ins, uint32_t delays_per_exec) {
//     this->num_cpu = num_cpu;
//     this->scheduler = scheduler; 
//     this->quantum_cycles = quantum_cycles; 
//     this->batch_process_freq = batch_process_freq; 
//     this-> min_ins = min_ins;
//     this-> max_ins = max_ins;
//     this-> delays_per_exec = delays_per_exec;

//     threadPool = std::make_unique<ThreadPool>(num_cpu);
// }

void Scheduler::setBatch(){
    this->isBatchProcess = true;
}

// Schedule a process
void Scheduler::scheduleProcess(const std::shared_ptr<Process>& process) {

    threadPool->enqueue([process]() {
        process->initProcess();  // Run the process's initialization
    });
}

