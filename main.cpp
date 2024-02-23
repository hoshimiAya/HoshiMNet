
#include <condition_variable>
#include <iostream>

#include "hoshiMNet/base/Log.h"

int main()
{
    hoshiMNet::Log::instance().log("Hello, spdlog!");
    LOG("Hello, spdlog!");
    LOG_WARN("Hello, spdlog!");
    LOG_ERROR("Hello, spdlog!");
    return 0;
}