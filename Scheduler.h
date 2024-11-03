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
    static void initialize(const MainConsole::Config& config);	

    void setupScheduler(uint8_t num_cpu, String scheduler, uint32_t quantum_cycles, uint32_t batch_process_freq, uint32_t min_ins, uint32_t max_ins, uint32_t delays_per_exec);
    void scheduleProcess(const std::shared_ptr<Process>& process);
    void setBatch(bool status);
    void shutdown(); 
    

private:
    MainConsole::Config config;

    Scheduler(const MainConsole::Config& config);																// Constructor
	~Scheduler() = default;
    
    static Scheduler* sched;											
 
    void generateBatchProcess(const MainConsole::Config& config, int i); 
    void processThread(int coreID); 

    ThreadPool threadPool;
    bool isShuttingDown = false;
    int cycleCounter = 0;                                  // Counter for CPU cycles
    bool isBatchProcess;
    
};