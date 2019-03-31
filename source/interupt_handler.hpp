#include <condition_variable>
#include <mutex>
#include <iostream>
#include <signal.h>

static std::condition_variable _condition;
static std::mutex _mutex;

class InterruptHandler {
public:
    static void hookSIGINT() {
        signal(SIGINT, handleUserInterrupt);
    }

    static void handleUserInterrupt(int signal) {
        if (signal == SIGINT) {
            std::cout << "SIGINT trapped ..." << "\n";
            _condition.notify_one();
        }
    }

    static void waitForUserInterrupt() {
        std::unique_lock<std::mutex> lock { _mutex };
        _condition.wait(lock);
        std::cout << "User has signaled to interrupt program..." << "\n";
        lock.unlock();
    }
};
