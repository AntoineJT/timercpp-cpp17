#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <functional>

class Timer {
	std::atomic<bool> active{true};
	
    public:
        void setTimeout(std::function<void()> callback, int delay);
        void setInterval(std::function<void()> callback, int interval);
        void stop();

};

void Timer::setTimeout(std::function<void()> callback, int delay) {
    active = true;
    std::thread t([=]() {
        if(!active.load()) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if(!active.load()) return;
        callback();
    });
    t.detach();
}

void Timer::setInterval(std::function<void()> callback, int interval) {
    active = true;
    std::thread t([=]() {
        while(active.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            if(!active.load()) return;
            callback();
        }
    });
    t.detach();
}

void Timer::stop() {
    active = false;
}
