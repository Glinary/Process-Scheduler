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
    : isShuttingDown(false), config(config), isBatchProcess(false), nextKey(0)
{
    std::thread([this, config = this->config, cycleCounter = this->cycleCounter]() mutable {

        workers.resize(config.num_cpu);  // Resize the vector to hold the required number of workers

        for (size_t i = 0; i < config.num_cpu; i++) {
            workers[i].isBusy = false;  // Initialize worker state
            workers[i].worker = std::thread([this, i] { this->worker(i); });
        }
        int batchProcessCount = 1;

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

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (!workers[nextKey].isBusy) {
            workers[nextKey].cv.notify_one();  
        }
    }

    if (!tasks.empty()) {
        nextKey = (nextKey + 1) % workers.size();  // Loop around available core
    }
}


void Scheduler::worker(int index) {
    while (!stop) {
        std::function<void()> task; 
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            while (tasks.empty() || workers[index].isBusy) {
                workers[index].cv.wait(lock);  
            }

            if (!tasks.empty()) {
                task = std::move(tasks.front());  
                tasks.pop();
                workers[index].isBusy = true;                       // occupy thread
            }
        }

        if (task) {
            task();  // Execute the task

            // free up thread
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                workers[index].isBusy = false;  
                workers[index].cv.notify_one();                         // next task, if waiting
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

    int nextK = nextKey;
    std::cout << nextKey << std::endl; 
    enqueue([process, nextK]() {
        process->initProcess(nextK);  // Run the process's initialization
    });
}

