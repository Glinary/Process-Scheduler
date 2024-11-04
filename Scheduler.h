#pragma once
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include "Process.h"
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <condition_variable>
#include <atomic>

#pragma once
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include "Process.h"
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <condition_variable>
#include <atomic>

class Scheduler {
public:
    static Scheduler* getInstance();
    static void initialize(const MainConsole::Config& config);
    void scheduleProcess(const std::shared_ptr<Process>& process);

    
    void setBatch(bool status);
    int getCycle();

    ~Scheduler(); // Destructor for cleanup

private:
    MainConsole::Config config;
    Scheduler(const MainConsole::Config& config);
    
    void enqueue(std::function<void()> task);
    void worker(); // Worker thread function
    void generateBatchProcess(const MainConsole::Config& config, int batchProcessCount);

    static Scheduler* sched;

    bool isShuttingDown;
    bool isBatchProcess;
    bool stop; // Flag to stop workers
    int cycleCounter; // Counter for cycle ticks
    int batchProcessCount = 1;
    int nextKey = 0;

    std::vector<std::thread> workers; // Vector of worker threads
    std::queue<std::function<void()>> tasks; // Task queue

    std::mutex queueMutex; // Mutex for protecting access to the task queue
    std::condition_variable cv; // Condition variable for signaling tasks
};