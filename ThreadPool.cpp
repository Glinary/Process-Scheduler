#include "ThreadPool.h"
#include "Scheduler.h"

ThreadPool* ThreadPool::threadpool = nullptr;				

ThreadPool* ThreadPool::getInstance() {
    return threadpool;
}

void ThreadPool::initialize(uint8_t num_cpu, std::string scheduler, uint32_t quantum_cycles) {
    threadpool = new ThreadPool(num_cpu, scheduler, quantum_cycles);
}

ThreadPool::ThreadPool() 
    : stop(false), num_cpu(0), scheduler("fcfs"), quantum_cycles(0), nextKey(0) {
}

ThreadPool::ThreadPool(uint8_t num_cpu, std::string scheduler, uint32_t quantum_cycles) 
    : stop(false), num_cpu(num_cpu), scheduler(scheduler), quantum_cycles(quantum_cycles), nextKey(0) {
    workers.resize(num_cpu);  // Resize the vector to hold the required number of workers

    for (size_t i = 0; i < num_cpu; i++) {
        workers[i].isBusy = false;  // Initialize worker state
        workers[i].worker = std::thread([this, i] { this->worker(i); });
    }
}


ThreadPool::~ThreadPool() {
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

void ThreadPool::enqueue(std::function<void()> task) {
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

void ThreadPool::worker(int index) {
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

int ThreadPool::getNextKey() {
    return nextKey;
}
