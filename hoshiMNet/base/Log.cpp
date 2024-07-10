#include "Log.h"

#include <iostream>
#include <sstream>
#include <thread>

#include "base/Singleton.h"

using namespace hoshiMNet;

void Log::log(std::string msg, int level)
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    std::string threadId = ss.str();
    if (threadId.size() > 6)
    {
        threadId = threadId.substr(threadId.size() - 6);
    }

    msg = std::string("[threadId:") + threadId + "] " + msg;

    switch (level)
    {
        std::cout << msg << std::endl;
    }
}

Log::Log()
{
}