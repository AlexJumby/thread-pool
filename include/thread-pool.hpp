#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <queue>
#include <sstream>
#include <functional>
#include <condition_variable>
#include <atomic>

struct Task {
    std::string name;
    std::function<void()> func;
};

class ThreadPool {
public:
    ThreadPool(size_t num_threads) : stop_(false) {
        worker_states_.resize(num_threads);
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this, i] {
                for(;;) {
                    std::unique_lock<std::mutex> lock(queue_mutex_);
                    condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                    if(stop_ && tasks_.empty())
                        return;
                    Task task = std::move(tasks_.front()); // Достаем таску из листа
                    tasks_.pop(); // Убираем таску по мере выполнения
                    lock.unlock(); // Разлочиваем мьютекс, чтобы другой поток мок перехватиь таску
                    {
                        std::lock_guard<std::mutex> lock(monitor_mutex_);
                        worker_states_[i].current_task = task.name;
                    }
                    task.func(); // Выполняем таску
                    {
                        std::lock_guard<std::mutex> lock(monitor_mutex_);
                        worker_states_[i].current_task = "";
                    }
                    completed_count_++;
                }
            });
        } 
    }

    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            stop_ = true;
            std::queue<Task> empty;
            std::swap(tasks_, empty);
        }
        condition_.notify_all();
        for (auto& w : workers_) {
            if (w.joinable())
                w.join();
        }
    }

    template<class F>
    void enqueue(const std::string& name, F&& task) {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        tasks_.emplace(Task{name, std::forward<F>(task)});
        lock.unlock();
        condition_.notify_one();
    }

    ~ThreadPool() {
        if(!stop_) {
            {
                std::lock_guard<std::mutex> lock(queue_mutex_);
                stop_ = true;
            };
            condition_.notify_all();
            for (auto& w : workers_)
                if (w.joinable())
                    w.join();
        }
    }
    struct WorkerState {
    std::string current_task; 
    };

    std::vector<WorkerState> get_worker_states() {
        std::lock_guard<std::mutex> lock(monitor_mutex_);
        return worker_states_;
    }

    size_t get_queue_size() {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        return tasks_.size();
    }
    
    size_t get_completed_count() {
        return completed_count_.load();
    }

private:
std::vector<WorkerState> worker_states_;
std::vector<std::thread> workers_;
std::queue<Task> tasks_;

std::mutex queue_mutex_;
std::mutex monitor_mutex_;
std::condition_variable condition_;
std::atomic<bool> stop_;
std::atomic<size_t> completed_count_{0};
    
};