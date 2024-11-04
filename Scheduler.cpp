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
    : isShuttingDown(false), config(config), isBatchProcess(false), cycleCounter(0), stop(false)
{
    // Resize the vector to hold the required number of worker threads
    workers.resize(config.num_cpu);

    // Launch worker threads
    for (size_t i = 0; i < config.num_cpu; i++) {
        workers[i] = std::thread(&Scheduler::worker, this);
    }


    // Background thread for generating batch processes
    std::thread([this, config]() mutable {
        while (!isShuttingDown) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            if (cycleCounter % config.batch_process_freq == 0 && isBatchProcess) {
                generateBatchProcess(config, batchProcessCount);
                batchProcessCount++;
            }

            cycleCounter++;
        }
    }).detach();
}

void Scheduler::enqueue(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        tasks.emplace(std::move(task));  // Add the task to the queue
    }

    // Notify one worker thread to process the task
    cv.notify_one();

    if (!tasks.empty()){
        nextKey = (nextKey + 1) % config.num_cpu;
    }
}

void Scheduler::worker() {
    while (!stop) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            // Wait for a task to be available
            cv.wait(lock, [this] { return !tasks.empty() || stop; });

            if (stop) return;  // Exit if stopping

            if (!tasks.empty()) {
                task = std::move(tasks.front());
                tasks.pop();
            }
        }

        if (task) {
            task();  // Execute the task
        }
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
void Scheduler::scheduleProcess(const std::shared_ptr<Process>& process) {
    int key = nextKey;
    enqueue([process, key] {
        process->initProcess(key);  // Run the process's initialization
    });
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
