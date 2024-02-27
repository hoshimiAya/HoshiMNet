#ifndef HOSHIMNET_BASE_THREADPOOL_H_
#define HOSHIMNET_BASE_THREADPOOL_H_

#include <atomic>
#include <functional>
#include <thread>
#include <vector>

namespace hoshiMNet
{
namespace base
{

class ThreadPool
{
public:

private:
    size_t threadNumber_;
    std::vector<std::thread> threads_;
    // 任务队列
    std::atomic<bool> stop_;
};

} // namespace base
} // namespace hoshiMNet

#endif // HOSHIMNET_BASE_THREADPOOL_H_