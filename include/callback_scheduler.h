#pragma once

#include <chrono>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <set>
#include <thread>

class CallbackScheduler {
public:
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
    using TaskId = std::uint64_t;

    CallbackScheduler();
    ~CallbackScheduler();

    CallbackScheduler(const CallbackScheduler&) = delete;
    CallbackScheduler& operator=(const CallbackScheduler&) = delete;

    TaskId Schedule(std::function<void()> callback, TimePoint when);
    bool Cancel(TaskId id);

private:
    struct Task {
        TaskId id;
        std::function<void()> callback;
        TimePoint when;

        bool operator>(const Task& other) const {
            return when > other.when;
        }
    };

    void WorkerLoop();

    std::mutex mutex_;
    std::condition_variable cv_;
    std::priority_queue<Task, std::vector<Task>, std::greater<Task>> tasks_;
    std::set<TaskId> cancelled_tasks_;

    TaskId next_id_ = 1;
    bool stop_ = false;
    std::thread worker_thread_;
};