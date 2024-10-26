#include "Scheduler.h"

class Scheduler {
public:
    void scheduleProcess(std::shared_ptr<Task> task) {
        processQueue.push(task);
    }

    void runScheduler() {
        while (!processQueue.empty()) {
            auto task = processQueue.front();
            processQueue.pop();
            task->initProcess(); // Execute the task
        }
    }

private:
    std::queue<std::shared_ptr<Task>> processQueue;
};