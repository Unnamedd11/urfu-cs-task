#pragma once

#include <chrono>
#include <cstdint>
#include <functional>

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

class CallbackScheduler
{
public:
    using TaskId = std::uint64_t;

    CallbackScheduler() = default;
    ~CallbackScheduler() = default;

    CallbackScheduler(const CallbackScheduler&) = delete;
    CallbackScheduler& operator=(const CallbackScheduler&) = delete;

    CallbackScheduler(CallbackScheduler&&) = delete;
    CallbackScheduler& operator=(CallbackScheduler&&) = delete;

    TaskId Schedule(std::function<void()> callback, TimePoint when);
    bool Cancel(TaskId id);
};
