#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <memory>

namespace Engine {
    class ThreadPool {
    public:
        ThreadPool(size_t numThreads = std::thread::hardware_concurrency());
        ~ThreadPool();

        template<typename F, typename... Args>
        auto Enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

        void Stop();
        size_t GetThreadCount() const { return workers.size(); }

    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queueMutex;
        std::condition_variable condition;
        bool stop;
    };

    template<typename F, typename... Args>
    auto ThreadPool::Enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
        
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> result = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex);

            if(stop) {
                throw std::runtime_error("ThreadPool is stopped");
            }

            tasks.emplace([task](){ (*task)(); });
        }

        condition.notify_one();

        return result;
    }
}
