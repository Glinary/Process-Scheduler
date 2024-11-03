#pragma once
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <condition_variable>
#include <atomic>
#include <string>
#include <iostream>

class ThreadPool {
public:
    ThreadPool();  // Default constructor (optional, can be deleted if not needed)
    ThreadPool(uint8_t num_cpu, std::string scheduler, uint32_t quantum_cycles);
    ~ThreadPool(); // Destructor

    static ThreadPool* getInstance();  // Singleton access
    static void initialize(uint8_t num_cpu, std::string scheduler, uint32_t quantum_cycles);

    void enqueue(std::function<void()> task);
    int getNextKey();

private:

    static ThreadPool* threadpool;	

    uint8_t num_cpu;
    std::string scheduler;
    uint32_t quantum_cycles;
    int nextKey;

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
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stop;

    void worker(int threadIndex);
};
