#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(uint8_t num_cpu, std::string scheduler, uint32_t quantum_cycles);
    ~ThreadPool();
    void enqueue(std::function<void()> task);
    int getNextKey();

    static ThreadPool* getInstance();  // Singleton access
    static void initialize(uint8_t num_cpu, std::string scheduler, uint32_t quantum_cycles);

private:
    static ThreadPool* threadpool;	

    uint8_t num_cpu;
    std::string scheduler;
    uint32_t quantum_cycles;

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
    void worker();

    int nextKey;
};
