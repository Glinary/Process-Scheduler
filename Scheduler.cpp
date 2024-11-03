#include "Scheduler.h"
#include "ConsoleManager.h"



Scheduler* Scheduler::sched = nullptr;				

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
        ThreadPool::initialize(config.num_cpu, config.scheduler, config.quantum_cycles);
        int batchProcessCount = 1;

        while (!isShuttingDown) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

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
        std::shared_ptr<BaseScreen> newScreen = std::make_shared<BaseScreen>(newProcess, processName);	

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

void Scheduler::setBatch(bool status){
    this->isBatchProcess = status;
}

// Schedule a process
void Scheduler::scheduleProcess(const std::shared_ptr<Process>& process) {

    int nextKey = ThreadPool::getInstance()->getNextKey();
    std::cout << nextKey << std::endl; 
    ThreadPool::getInstance()->enqueue([process, nextKey]() {
        process->initProcess(nextKey);  // Run the process's initialization
    });
}

