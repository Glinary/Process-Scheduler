#pragma once

#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include "Process.h"

class Scheduler {
public:
    static Scheduler* getInstance();
    static void initialize(const MainConsole::Config& config);
    void scheduleProcess(std::shared_ptr<Process>& process);
    

    void setBatch(bool status);
    int getCycle();
    void release_core(int coreID);


    ~Scheduler();  // Destructor for cleanup

private:
    explicit Scheduler(const MainConsole::Config& config);
    void enqueue(std::shared_ptr<Process> task);
    void worker();  // Worker thread function
    void generateBatchProcess(const MainConsole::Config& config, int batchProcessCount);
    int getAvailableCore();

    void roundRobinSchedule(std::shared_ptr<Process>& process);

    static Scheduler* sched;

    MainConsole::Config config;
    bool isShuttingDown = false;
    bool isBatchProcess = false;
    bool stop = false;  // Flag to stop workers
    int cycleCounter = 0;  // Counter for cycle ticks
    int batchProcessCount = 1;
    int nextKey = 0;

    std::vector<bool> coresStatus;
    std::mutex core_mutex;

    std::vector<std::thread> workers;  // Vector of worker threads
    std::queue<std::shared_ptr<Process>> tasks;  // Task queue
    std::queue<std::shared_ptr<Process>> rrQueue;

    std::mutex queueMutex;  // Mutex for protecting access to the task queue
    std::condition_variable cv;  // Condition variable for signaling tasks
};

