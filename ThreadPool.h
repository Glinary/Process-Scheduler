#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <string>

class ThreadPool {
public:
    // Constructor that initializes the thread pool with specified CPU count, scheduler type, and quantum cycles
    ThreadPool(uint8_t num_cpu, std::string scheduler, uint32_t quantum_cycles);
    ~ThreadPool();

    void enqueue(std::function<void()> task);  // Add a task to the queue
    int getNextKey();                          // Get the next key for scheduling

    static ThreadPool* getInstance();          // Singleton access
    static void initialize(uint8_t num_cpu, std::string scheduler, uint32_t quantum_cycles);  // Initialize singleton instance

private:
    static ThreadPool* threadpool;             // Singleton instance

    uint8_t num_cpu;                           // Number of CPUs
    std::string scheduler;                     // Scheduler type
    uint32_t quantum_cycles;                   // Quantum cycles for Round Robin

    std::vector<std::thread> workers;          // Worker threads
    std::queue<std::function<void()>> tasks;   // Task queue
    std::mutex queueMutex;                     // Mutex for queue access
    std::condition_variable condition;         // Condition variable for task synchronization
    bool stop;                                 // Flag to stop the pool
    int nextKey;                               // Key for scheduling in round-robin

    void worker();                             // Worker function for each thread
};
