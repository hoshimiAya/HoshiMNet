#include "Log.h"

#ifdef SPDLOG_ON
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#else
#include <iostream>
#endif // SPDLOG_ON

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
#ifdef SPDLOG_ON
    case 0:
        logger_->info(msg);
        break;
    case 1:
        logger_->warn(msg);
        break;
    case 2:
        logger_->error(msg);
        break;
    default:
        logger_->info(msg);
        break;
#else
    std::cout << msg << std::endl;
#endif // SPDLOG_ON
    }
}

Log::Log()
{
#ifdef SPDLOG_ON
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt", true));

    logger_ = std::make_shared<spdlog::logger>("logger", begin(sinks), end(sinks));
    spdlog::register_logger(logger_);

    logger_->set_level(spdlog::level::info);

    logger_->info("Log initialized");
#endif // SPDLOG_ON
}