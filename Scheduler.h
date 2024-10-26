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
#include "ThreadPool.h"

class Scheduler {
public:
    static Scheduler* getInstance();  // Singleton access
    static void initialize();	

    void setCore(int maxcpu);
    void scheduleProcess(const std::shared_ptr<Process>& process);
    void shutdown(); 
    

private:
    Scheduler();																// Constructor
	~Scheduler() = default;
    
    static Scheduler* scheduler;											

    void processThread(int coreID); 

    int maxCores;
    std::unique_ptr<ThreadPool> threadPool;
    bool isShuttingDown = false;
};