#include <iostream>
#include <thread>
#include <mutex>
#include <map>
#include <vector>
#include <sstream>
#include <queue>
#include <functional>
#include <condition_variable>
#include <chrono>  // For std::chrono::milliseconds

using namespace std;

class Screen {
public:
    explicit Screen(const string& name) : name(name) {}

    void print_message(const string& message) {
        lock_guard<mutex> lock(mutex_);
        prints.push_back(message);
    }

    void resume() {
        lock_guard<mutex> lock(mutex_);
        cout << "Screen " << name << ": " << prints.size() << " messages printed." << endl;
    }

private:
    const string name;
    mutex mutex_;
    vector<string> prints;
};

// A simple thread pool to simulate the scheduler using 4 CPU cores
class ThreadPool {
public:
    ThreadPool(size_t threads) {
        for (size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(queue_mutex);
                        condition.wait(lock, [this] { return !tasks.empty() || stop; });
                        if (stop && tasks.empty()) return;
                        task = move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    // Schedule a task to be executed by the pool
    void enqueue_task(function<void()> task) {
        {
            lock_guard<mutex> lock(queue_mutex);
            tasks.push(task);
        }
        condition.notify_one();
    }

    // Stop the pool and join all threads
    ~ThreadPool() {
        {
            lock_guard<mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (thread &worker : workers)
            worker.join();
    }

private:
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex queue_mutex;
    condition_variable condition;
    bool stop = false;
};

// Function to create a screen and print messages
void create_screen(const string& name, map<string, Screen*>& screens) {
    Screen* screen = new Screen(name);
    screens[name] = screen;

    for (int i = 0; i < 200; ++i) {
        screen->print_message("Message " + to_string(i) + " on " + name);
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    delete screen;
}

int main() {
    map<string, Screen*> screens;
    ThreadPool pool(4);  // A thread pool with 4 threads to simulate 4 CPU cores

    while (true) {
        string command;
        cout << "Enter command (screen -s <name> or screen -r <name>): ";
        getline(cin, command);

        if (command.empty()) {
            break;
        }

        istringstream iss(command);
        string action, option, name;
        iss >> action >> option >> name;

        if (action == "screen" && option == "-s") {
            pool.enqueue_task([name, &screens]() {
                create_screen(name, screens);
            });
        } else if (action == "screen" && option == "-r") {
            if (screens.count(name) > 0) {
                screens[name]->resume();
            } else {
                cout << "Screen " << name << " not found." << endl;
            }
        } else {
            cout << "Invalid command." << endl;
        }
    }

    return 0;
}
