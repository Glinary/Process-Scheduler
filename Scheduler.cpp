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
        // ThreadPool::initialize(config.num_cpu, config.scheduler, config.quantum_cycles);

        // create CPU Core
         for (size_t i = 0; i < config.num_cpu; i++) {
            workers[i].isBusy = false;  // Initialize worker state
            workers[i].worker = std::thread([this, i] { this->worker(i); });
        }



        // Scheduler-test (Create Batch Process)
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

Scheduler::~Scheduler() {
    stop = true;
    for (size_t i = 0; i < workers.size(); ++i) {
        workers[i].cv.notify_all();  // Wake up all threads
    }
    for (auto &worker : workers) {
        if (worker.worker.joinable()) {
            worker.worker.join();  // Wait for all threads to finish
        }
    }
}

void Scheduler::enqueue(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        tasks.emplace(std::move(task));  // Add the task to the queue
    }

    // Notify the specific worker thread associated with this key
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (!workers[nextKey].isBusy) {
            workers[nextKey].cv.notify_one();  // Notify the worker if it's not busy
        }
    }

    // Only increment nextKey after successfully adding a task
    if (!tasks.empty()) {
        nextKey = (nextKey + 1) % workers.size();  // Loop around available core
    }
}


void Scheduler::worker(int index) {
    while (!stop) {
        std::function<void()> task;  // Declare the task variable
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            while (tasks.empty() || workers[index].isBusy) {
                workers[index].cv.wait(lock);  // Wait until a task is available and not busy
            }

            if (!tasks.empty()) {
                task = std::move(tasks.front());  // Retrieve the task
                tasks.pop();
                workers[index].isBusy = true;  // Mark thread as busy
            }
        }

        if (task) {
            task();  // Execute the task
            // After the task completes, mark the thread as free
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                workers[index].isBusy = false;  // Mark as available
                workers[index].cv.notify_one();  // Notify any waiting tasks
            }
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

void Scheduler::setBatch(bool status){
    this->isBatchProcess = status;
}

int Scheduler::getNextKey() {
    return nextKey;
}


// Schedule a process
void Scheduler::scheduleProcess(const std::shared_ptr<Process>& process) {

    int nextKey = nextKey;
    std::cout << nextKey << std::endl; 
    Scheduler::getInstance()->enqueue([process, nextKey]() {
        process->initProcess(nextKey);  // Run the process's initialization
    });
}

