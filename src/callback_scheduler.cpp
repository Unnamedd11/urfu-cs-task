#include "callback_scheduler.h"
#include <iostream>

CallbackScheduler::CallbackScheduler() {
    worker_thread_ = std::thread(&CallbackScheduler::WorkerLoop, this);
}

CallbackScheduler::~CallbackScheduler() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stop_ = true;
    }
    cv_.notify_all();
    if (worker_thread_.joinable()) {
        worker_thread_.join();
    }
}

CallbackScheduler::TaskId CallbackScheduler::Schedule(std::function<void()> callback, TimePoint when) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    TaskId id = next_id_++;
    tasks_.push(Task{id, std::move(callback), when});
    
    cv_.notify_one(); 
    return id;
}

bool CallbackScheduler::Cancel(TaskId id) {
    std::lock_guard<std::mutex> lock(mutex_);
    cancelled_tasks_.insert(id);
    return true; 
}

void CallbackScheduler::WorkerLoop() {
    while (true) {
        std::unique_lock<std::mutex> lock(mutex_);

        while (tasks_.empty() && !stop_) {
            cv_.wait(lock);
        }

        if (stop_) {
            return;
        }

        auto now = std::chrono::system_clock::now();
        auto closest_task_time = tasks_.top().when;

        if (now >= closest_task_time) {
            Task task = std::move(const_cast<Task&>(tasks_.top()));
            tasks_.pop();

            auto it = cancelled_tasks_.find(task.id);
            if (it != cancelled_tasks_.end()) {
                cancelled_tasks_.erase(it);
                continue;
            }

            lock.unlock(); 

            try {
                if (task.callback) {
                    task.callback();
                }
            } catch (...) {
            }
        } else {
            cv_.wait_until(lock, closest_task_time);
        }
    }
}