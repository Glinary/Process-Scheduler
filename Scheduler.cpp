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
    : isShuttingDown(false), config(config), isBatchProcess(false), cycleCounter(0), stop(false), coresStatus(config.num_cpu, false)
{
    // Resize the vector to hold the required number of worker threads
    workers.resize(config.num_cpu);

    std::cout << config.scheduler << std::endl;

    // Launch worker threads
    for (size_t i = 0; i < config.num_cpu; i++) {
        workers[i] = std::thread(&Scheduler::worker, this);
    }


    // Background thread for generating batch processes
    std::thread([this, config]() mutable {
        while (!isShuttingDown) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            if (cycleCounter % config.batch_process_freq == 0 && isBatchProcess) {
                generateBatchProcess(config, batchProcessCount);
                batchProcessCount++;
            }

            cycleCounter++;
        }
    }).detach();
}

void Scheduler::enqueue(const std::shared_ptr<Process> task) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (config.scheduler == "rr") {
            rrQueue.push(task);
        } else {
            tasks.push(task);
        }
    }
    cv.notify_one();  // Notify a worker thread
}

int Scheduler::getAvailableCore(){
        std::lock_guard<std::mutex> lock(core_mutex);

         for (int i = 0; i < config.num_cpu; ++i) {
            if (!coresStatus[i]) {  
                coresStatus[i] = true;  // Mark it as busy
                return i;  
            }
        }
        return -1;  // All cores are busy
}

void Scheduler::release_core(int coreID) {
    std::lock_guard<std::mutex> lock(core_mutex);
    coresStatus[coreID] = false;  // Mark the core as available again
}


void Scheduler::worker() {
    while (!stop) {
        std::shared_ptr<Process>  task;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            // Wait for a task to be available
            cv.wait(lock, [this] { return !tasks.empty() || !rrQueue.empty() || stop; });

            if (stop) return;  // Exit if stopping

            if(config.scheduler == "rr" && !rrQueue.empty()){
                task = std::move(rrQueue.front());
                rrQueue.pop();
            } else if (config.scheduler == "fcfs" && !tasks.empty())
            {
                task = std::move(tasks.front());
                tasks.pop();
            }
        }

        if (task) {
             if (config.scheduler == "fcfs") {
                task->initProcess(getAvailableCore());
            } else if (config.scheduler == "rr") {
                roundRobinSchedule(task);
            }
        }
    }
}

void Scheduler::roundRobinSchedule(std::shared_ptr<Process>& process) {
    int remainingTime = config.quantum_cycles;
    int coreID = getAvailableCore();

    while (remainingTime > 0 && !process->getIsFinished()) {
        process->executeInstruction(getAvailableCore());  // Execute one instruction (simulates one cycle)
        remainingTime--;
    }

    release_core(coreID);  // Release the core once the time quantum is exhausted

    if (!process->getIsFinished()) {
        std::lock_guard<std::mutex> lock(queueMutex);
        rrQueue.push(process);  // Re-enqueue the process if it’s not complete
        cv.notify_one();
    }
}

void Scheduler::generateBatchProcess(const MainConsole::Config& config, int batchProcessCount) {
    String processName = "Process" + std::to_string(batchProcessCount);
    std::shared_ptr<Process> newProcess = std::make_shared<Process>(processName, config);
    std::shared_ptr<BaseScreen> newScreen = std::make_shared<BaseScreen>(newProcess, processName);	

    scheduleProcess(newProcess);

    ConsoleManager::getInstance()->addProcesses(newProcess);
    ConsoleManager::getInstance()->registerScreen(newScreen);
}

void Scheduler::setBatch(bool status) {
    this->isBatchProcess = status;
}

int Scheduler::getCycle() {
    return cycleCounter;
}

// Schedule a process
void Scheduler::scheduleProcess(std::shared_ptr<Process>& process) {
    enqueue(process);
}

Scheduler::~Scheduler() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;  // Signal all workers to stop
    }
    cv.notify_all();  // Wake all worker threads

    // Join all worker threads
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}
