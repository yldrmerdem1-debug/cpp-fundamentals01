#pragma once
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

// Fixed-size pool of worker threads. submit() returns a std::future for the task's result;
// an exception thrown by a task is delivered through that future instead of killing a worker.
class ThreadPool {
public:
    explicit ThreadPool(std::size_t threads = std::thread::hardware_concurrency()) {
        if (threads == 0) threads = 1;  // hardware_concurrency() may return 0
        workers_.reserve(threads);
        for (std::size_t i = 0; i < threads; ++i) workers_.emplace_back([this] { workerLoop(); });
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ~ThreadPool() { shutdown(); }

    template <typename F, typename... Args>
    auto submit(F&& f, Args&&... args)
        -> std::future<std::invoke_result_t<std::decay_t<F>&, std::decay_t<Args>...>> {
        using Result = std::invoke_result_t<std::decay_t<F>&, std::decay_t<Args>...>;
        auto task = std::make_shared<std::packaged_task<Result()>>(
            [fn = std::forward<F>(f), params = std::make_tuple(std::forward<Args>(args)...)]() mutable {
                return std::apply(fn, std::move(params));
            });
        std::future<Result> result = task->get_future();
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (stopping_) throw std::runtime_error("submit() on a stopped ThreadPool");
            tasks_.emplace([task] { (*task)(); });
        }
        cv_.notify_one();
        return result;
    }

    // Runs every task that is already queued, then joins the workers. Safe to call twice.
    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stopping_ = true;
        }
        cv_.notify_all();
        for (auto& worker : workers_) {
            if (worker.joinable()) worker.join();
        }
    }

    std::size_t size() const { return workers_.size(); }

private:
    void workerLoop() {
        for (;;) {
            std::function<void()> job;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                cv_.wait(lock, [this] { return stopping_ || !tasks_.empty(); });
                if (stopping_ && tasks_.empty()) return;
                job = std::move(tasks_.front());
                tasks_.pop();
            }
            job();  // run outside the lock so other workers can take tasks
        }
    }

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stopping_ = false;
};
