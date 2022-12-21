#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <string>

#include "SimpleThreadPool.h"

size_t simple_task() {
    std::cout << "start simple job\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "done simple job\n";
    return 2 + 2;
}

int main() {

    std::vector< std::future<size_t> > results;
    {
        const int threads_amount {5};
        SimpleThreadPool pool(threads_amount);

        // Launching thread pool
        // Acquiring tasks futures
        for (auto i = 0; i < (threads_amount*2); i++) {
            results.push_back(
                pool.Post(simple_task)
            );
        }
    }

    // Printing results
    for(auto& r: results) {
        std::cout << '\n' << r.get();
    }

    return 0;
}