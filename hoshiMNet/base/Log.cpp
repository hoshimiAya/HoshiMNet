#include "Log.h"

#include <spdlog/spdlog.h>

#include "base/Singleton.h"

using namespace hoshiMNet;

void Log::log(std::string msg, int level)
{
    switch (level)
    {
    case 0:
        spdlog::info(msg);
        break;
    case 1:
        spdlog::warn(msg);
        break;
    case 2:
        spdlog::error(msg);
        break;
    default:
        spdlog::info(msg);
        break;
    }
}