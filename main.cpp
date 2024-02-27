
#include <condition_variable>
#include <iostream>

#include "hoshiMNet/base/Log.h"

#include "hoshiMNet/base/MpmcQueue.h"

void testLog()
{
    hoshiMNet::Log::instance().log("Hello, spdlog!");
    LOG("Hello, spdlog!");
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

int main()
{
    testLog();
    testMpmcQueue();
    return 0;
}