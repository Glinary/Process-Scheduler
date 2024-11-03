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

    static Scheduler* getInstance();  // Singleton access
    static void initialize(const MainConsole::Config& config);	

    void scheduleProcess(const std::shared_ptr<Process>& process);
    int getCycle();
    void setBatch(bool status);
    void shutdown(); 

    
private:
    MainConsole::Config config;

    Scheduler(const MainConsole::Config& config);			// Constructor
	~Scheduler() = default;
    
    static Scheduler* sched;											
 
    void generateBatchProcess(const MainConsole::Config& config, int i); 
    void enqueue(std::function<void()> task);
    int getNextKey();

    bool isShuttingDown = false;
    int cycleCounter = 0;                                  // Counter for CPU cycles
    bool isBatchProcess;

    struct ThreadInfo {
        std::thread worker;
        std::atomic<bool> isBusy;
        std::condition_variable cv;

        ThreadInfo() : isBusy(false) {}
        ThreadInfo(const ThreadInfo&) = delete;
        ThreadInfo& operator=(const ThreadInfo&) = delete;
        ThreadInfo(ThreadInfo&& other) noexcept : worker(std::move(other.worker)), isBusy(other.isBusy.load()) {}
    };

    void worker(int threadIndex);
    std::vector<ThreadInfo> workers; // Store thread info
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stop;
    int nextKey;
};