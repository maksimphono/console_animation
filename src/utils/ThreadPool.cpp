#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

class ThreadPool {
public:
    ThreadPool(size_t num_threads) : stop(false) {
        for (size_t i = 0; i < num_threads; ++i) {
            threads.emplace_back([this] {
                while (true) {
                    std::function<void()> task; 
                    { 
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] {
                            return this->stop || !this->tasks.empty();
                        });
                        if (this->stop && this->tasks.empty()) {
                            return;
                        }
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    } 
                    try {
                        task();
                    } catch (const std::exception& e) {
                        std::cerr << "Thread ID " << std::this_thread::get_id()
                                  << " caught exception during task execution: " << e.what() << std::endl;
                    } catch (...) {
                        std::cerr << "Thread ID " << std::this_thread::get_id()
                                  << " caught unknown exception during task execution." << std::endl;
                    }
                }
            });
        }
    }
    ~ThreadPool() {
        { 
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true; 
        }
        condition.notify_all(); 
        for (std::thread& worker : threads) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
    template<class F, class... Args>
    auto submit(F&& f, Args&&... args) -> std::future<std::result_of_t<F(Args...)>> {
        using return_type = std::result_of_t<F(Args...)>;
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        std::future<return_type> res = task->get_future();
        { 
            std::unique_lock<std::mutex> lock(queue_mutex);
            if (stop) {
                throw std::runtime_error("submit on stopped ThreadPool");
            }
            tasks.emplace([task]() {
                (*task)(); 
            });
        }
        condition.notify_one(); 
        return res; 
    }
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};