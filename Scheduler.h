#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <atomic>
#include <memory>
#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include "Process.h"

class Scheduler {
public:

    static Scheduler* getInstance();  // Singleton access
    static void initialize(const MainConsole::Config& config);	

    void scheduleProcess(const std::shared_ptr<Process>& process);
    void setBatch(bool status);
    void shutdown(); 

    void enqueue(std::function<void()> task);
    int getNextKey();
    

private:
    MainConsole::Config config;

    Scheduler(const MainConsole::Config& config);																// Constructor
    
    static Scheduler* sched;											
 
    void generateBatchProcess(const MainConsole::Config& config, int i); 
    void processThread(int coreID); 

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

    std::vector<ThreadInfo> workers; // Store thread info
    void worker(int threadIndex);
    std::atomic<bool> stop;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    int nextKey;
};