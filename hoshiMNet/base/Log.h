#ifndef HOSHIMNET_LOG_H_
#define HOSHIMNET_LOG_H_

#include <string>

#ifdef SPDLOG_ON
#include <spdlog/spdlog.h>
#endif // SPDLOG_ON

#include "base/Singleton.h"


#define LOG_INFO(msg) hoshiMNet::Log::instance().log(msg)
#define LOG_WARN(msg) hoshiMNet::Log::instance().log(msg, 1)
#define LOG_ERROR(msg) hoshiMNet::Log::instance().log(msg, 2)

namespace hoshiMNet
{

class Log : public base::Singleton<Log>
{
public:
    void log(std::string msg, int level = 0);

private:
    Log();
    ~Log() {}
    friend class base::Singleton<Log>;

#ifdef SPDLOG_ON
    std::shared_ptr<spdlog::logger> logger_;
#endif
};

} // namespace hoshiMNet

#endif // HOSHIMNET_LOG_H_