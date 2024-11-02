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
#include "ConsoleManager.h"
#include "ThreadPool.h"

class Scheduler {
public:

    static Scheduler* getInstance();  // Singleton access
    static void initialize(const MainConsole::Config& config);	

    void setupScheduler(uint8_t num_cpu, String scheduler, uint32_t quantum_cycles, uint32_t batch_process_freq, uint32_t min_ins, uint32_t max_ins, uint32_t delays_per_exec);
    void scheduleProcess(const std::shared_ptr<Process>& process);
    void setBatch();
    void shutdown(); 
    

private:
    MainConsole::Config config;

    Scheduler(const MainConsole::Config& config);																// Constructor
	~Scheduler() = default;
    
    static Scheduler* sched;											
 
    void generateBatchProcess(const MainConsole::Config& config, int i); 
    void processThread(int coreID); 

    uint8_t num_cpu;					// Number of CPUs: [1, 128]
    String scheduler;					// or enum SchedulerType { FCFS, RR };
    uint32_t quantum_cycles;			// For Round Robin: [1, 2^32]
    uint32_t batch_process_freq;		// Frequency of batch process: [1, 2^32]
    uint32_t min_ins;					// Minimum instructions: [1, 2^32]
    uint32_t max_ins;					// Maximum instructions: [1, 2^32]
    uint32_t delays_per_exec;			// Delays per execution: [0, 2^32]
    std::unique_ptr<ThreadPool> threadPool;
    bool isShuttingDown = false;
    int cycleCounter = 0;                                  // Counter for CPU cycles
    uint32_t batchProcessFreq;   

    bool isBatchProcess;
    
};