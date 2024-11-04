#include "ThreadPool.h"
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <condition_variable>
#include <atomic>

ThreadPool* ThreadPool::threadpool = nullptr; // Static instance of ThreadPool

ThreadPool* ThreadPool::getInstance() {
    return threadpool;
}

void ThreadPool::initialize(uint8_t num_cpu, std::string scheduler, uint32_t quantum_cycles) {
    if (!threadpool) { // Ensure only one instance is created
        threadpool = new ThreadPool(num_cpu, scheduler, quantum_cycles);
    }
}

ThreadPool::ThreadPool(uint8_t num_cpu, std::string scheduler, uint32_t quantum_cycles)
    : stop(false), num_cpu(num_cpu), scheduler(std::move(scheduler)), quantum_cycles(quantum_cycles), nextKey(0) {
    workers.reserve(num_cpu); // Reserve space for the number of workers
    for (size_t i = 0; i < num_cpu; ++i) {
        workers.emplace_back([this] { this->worker(); });
    }
}

ThreadPool::~ThreadPool() {
    stop = true; // Signal all workers to stop
    condition.notify_all(); // Wake up all threads
    for (std::thread &worker : workers) {
        worker.join(); // Wait for each worker to finish
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(queueMutex); // Lock the queue for safe access
        tasks.emplace(std::move(task)); // Move the task into the queue
    }
    condition.notify_one(); // Notify one waiting worker

    // Only update nextKey if tasks were added
    if (!tasks.empty()) {
        nextKey = (nextKey + 1) % num_cpu; // Update the next key in a round-robin fashion
    }
}

void ThreadPool::worker() {
    while (!stop) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            // Wait until there's a task or stop signal
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            if (stop && tasks.empty())
                return;

            task = std::move(tasks.front()); // Get the task
            tasks.pop(); // Remove it from the queue
        }
        task(); // Execute the task
    }
}

int ThreadPool::getNextKey() {
    return nextKey; // Return the current next key
}
