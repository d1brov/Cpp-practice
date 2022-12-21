#ifndef SIMPLE_THREAD_POOL_H
#define SIMPLE_THREAD_POOL_H

#include <thread>
#include <future>
#include <vector>
#include <queue>

class SimpleThreadPool {
public:
    // Creates thread pool with thread_count threads
    explicit SimpleThreadPool(std::size_t thread_count);

    // Deletes thread pool (after all postet tasks completion)
    // After destructor call posting of new tasks not available
    ~SimpleThreadPool();

    // Frobids copying thread pool objects
    SimpleThreadPool(const SimpleThreadPool & ) = delete;
    SimpleThreadPool & operator = (const SimpleThreadPool & ) = delete;

    // Adds tasks to queue and returns future to its result.
    // Launches working loop on first call.
    template <typename Function_T> 
    auto Post(Function_T function) -> std::future <decltype(function())>;

private:
    // Gets task from queue for available thread to WorkOn it
    void WorkOn();

private:
    size_t m_thread_count;
    std::vector < std::thread > m_threads;
    std::queue < std::function <void()>> m_tasks;
    std::condition_variable m_cv;
    std::mutex m_mutex;
    bool m_stop;
};

///////////////////////////////////////////////
// IMPLEMENTATION SHOULD BE IN SEPARATE .cpp //
///////////////////////////////////////////////

SimpleThreadPool::SimpleThreadPool(size_t thread_count) : 
    m_stop(false),
    m_thread_count(thread_count) {
        for (size_t i = 0; i < thread_count; i++) {
            m_threads.emplace_back(&SimpleThreadPool::WorkOn, this);
        }
}

SimpleThreadPool::~SimpleThreadPool() {
    m_stop = true;
    m_cv.notify_all();
    for(std::thread& t: m_threads) {
        t.join();
    }
}

template <typename Function_T>
auto SimpleThreadPool::Post(Function_T function) -> std::future<decltype(function())> {
    using return_type = typename std::result_of<Function_T()>::type;
    auto p_pkg_task = std::make_shared<std::packaged_task<return_type()>> (function);
    std::future<return_type> future_result = p_pkg_task->get_future();

    {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        if(m_stop)
            throw std::runtime_error("Cannot add to stopped thread pool");

        m_tasks.emplace(
            [p_pkg_task](){(*p_pkg_task)();}
        );
    }
    m_cv.notify_one();

    return future_result;
}

void SimpleThreadPool::WorkOn() {
    while(true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(this->m_mutex);
            this->m_cv.wait(lock, 
                [this]() -> bool {
                    return this->m_stop || !this->m_tasks.empty(); 
                }
            );

            if(this->m_stop && this->m_tasks.empty())
                return;

            task = std::move(this->m_tasks.front());
            this->m_tasks.pop();
        }
        task();
    }
}

#endif