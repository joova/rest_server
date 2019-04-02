#ifndef REST_SERVER_INTERRUPT_HANDLER_H_
#define REST_SERVER_INTERRUPT_HANDLER_H_


#include <condition_variable>
#include <mutex>
#include <iostream>
#include <signal.h>

static std::condition_variable _condition;
static std::mutex _mutex;

class InterruptHandler {
public:
    static void hookSIGINT() {
        signal(SIGINT, HandleUserInterrupt);
    }

    static void HandleUserInterrupt(int signal) {
        if (signal == SIGINT) {
            std::cout << "SIGINT trapped ..." << "\n";
            _condition.notify_one();
        }
    }

    static void WaitForUserInterrupt() {
        std::unique_lock<std::mutex> lock { _mutex };
        _condition.wait(lock);
        std::cout << "User has signaled to interrupt program..." << "\n";
        lock.unlock();
    }
};

#endif // REST_SERVER_INTERRUPT_HANDLER_H_
