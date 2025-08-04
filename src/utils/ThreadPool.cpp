#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional> // For std::function
#include <future>     // For std::future, std::packaged_task

// A simple ThreadPool class
class ThreadPool {
public:
    // Constructor: Initializes the thread pool with a specified number of worker threads.
    ThreadPool(size_t num_threads) : stop(false) {
        // Create worker threads and start their execution loops
        for (size_t i = 0; i < num_threads; ++i) {
            threads.emplace_back([this] {
                // Each thread runs this lambda function
                // It continuously tries to get tasks from the queue
                while (true) {
                    std::function<void()> task; // Placeholder for the task to execute

                    { // Scope for unique_lock to manage mutex lifetime
                        std::unique_lock<std::mutex> lock(this->queue_mutex);

                        // Wait until there's a task or the pool is stopping
                        // The predicate (lambda) prevents spurious wakeups
                        this->condition.wait(lock, [this] {
                            return this->stop || !this->tasks.empty();
                        });

                        // If the pool is stopping AND there are no more tasks, exit the thread loop
                        if (this->stop && this->tasks.empty()) {
                            return;
                        }

                        // Get the next task from the queue
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    } // Mutex is released here

                    // Execute the task outside the lock to allow other threads to access the queue
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

    // Destructor: Signals all threads to stop and waits for them to finish.
    ~ThreadPool() {
        { // Acquire lock to modify 'stop' flag and notify all
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true; // Set stop flag to true
        }
        condition.notify_all(); // Notify all waiting threads to check the stop flag

        // Join all worker threads to ensure they complete their execution
        // and resources are properly cleaned up.
        for (std::thread& worker : threads) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    // Submits a task to the thread pool.
    // F is the function type, Args are its arguments.
    // Returns a std::future to get the result of the task.
    template<class F, class... Args>
    auto submit(F&& f, Args&&... args) -> std::future<std::result_of_t<F(Args...)>> {
        // Define the return type of the function f with its arguments
        using return_type = std::result_of_t<F(Args...)>;

        // Create a packaged_task from the function and its arguments.
        // packaged_task wraps a callable object and allows its result to be retrieved via a future.
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        // Get the future associated with this packaged_task
        std::future<return_type> res = task->get_future();

        { // Acquire lock to push the task to the queue
            std::unique_lock<std::mutex> lock(queue_mutex);

            // Ensure we're not submitting tasks to a stopped pool
            if (stop) {
                throw std::runtime_error("submit on stopped ThreadPool");
            }

            // Push the packaged_task (as a void function) into the queue
            // The lambda ensures the task is executed when popped by a worker
            tasks.emplace([task]() {
                (*task)(); // Execute the packaged_task
            });
        }
        condition.notify_one(); // Notify one waiting worker thread that a new task is available
        return res; // Return the future to the caller
    }

private:
    // Vector to hold all worker threads
    std::vector<std::thread> threads;
    // Queue to store tasks (packaged_tasks wrapped in std::function<void()>)
    std::queue<std::function<void()>> tasks;

    // Mutex to protect access to the task queue
    std::mutex queue_mutex;
    // Condition variable to signal worker threads about new tasks or shutdown
    std::condition_variable condition;
    // Flag to indicate if the pool should stop
    bool stop;
};