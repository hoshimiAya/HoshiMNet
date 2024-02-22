
#include <condition_variable>
#include <iostream>

// #include <spdlog/spdlog.h>

#include "hoshiMNet/temp.h"

int main()
{
    std::condition_variable cv;

    Test t;
    int result = t.sum(1, 2);
    std::cout << "result: " << result << std::endl;
    // spdlog::info("result: {}", result);
    return 0;
}