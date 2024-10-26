#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include "Task.h"

// Scheduler class
class Scheduler {
public:
    void scheduleProcess(std::shared_ptr<Task> task);
    void runScheduler();

private:
    std::queue<std::shared_ptr<Task>> processQueue;
};