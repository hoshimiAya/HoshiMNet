
#include <chrono>
#include <condition_variable>
#include <thread>
#include <iostream>

#include "hoshiMNet/base/Log.h"
#include "hoshiMNet/base/MpmcQueue.h"
#include "hoshiMNet/base/ThreadPool.h"

void testLog()
{
    hoshiMNet::Log::instance().log("Hello, spdlog!");
    LOG_INFO("Hello, spdlog!");
    LOG_WARN("Hello, spdlog!");
    LOG_ERROR("Hello, spdlog!");
}

void testMpmcQueue()
{
    hoshiMNet::base::MpmcQueue<int> queue(10);
    
    for (int i = 0; i < 12; ++i)
    {
        std::cout << "size: " << queue.size() << ", i: " << i << std::endl;
        queue.push(std::move(i));
    }

    std::cout << "size: " << queue.size() << std::endl;
    std::cout << "maxSize: " << queue.maxSize() << std::endl;

    int value;
    for (int i = 0; i < 12; ++i)
    {
        queue.pop(value);
        std::cout << value << std::endl;
    }
}

void testThreadPool()
{
    hoshiMNet::base::ThreadPool pool(2, 10);
    pool.start();

    std::function<void(int)> task = [](int i)
    {
        // print thread
        std::string str = "thread id: " + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())) + ", task: " + std::to_string(i);
        LOG_INFO(str);
    };

    for (int i = 0; i < 100; ++i)
    {
        pool.post(std::bind(task, i));
        std::string str = "post: " + std::to_string(i);
        LOG_INFO(str);
    }

    pool.stop();
}

int main()
{
    // testLog();
    // testMpmcQueue();
    testThreadPool();
    return 0;
}