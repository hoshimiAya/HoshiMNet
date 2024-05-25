#include "EventLoopThreadPool.h"

#include "../base/Log.h"
#include "EventLoop.h"

using namespace hoshiMNet;
using namespace hoshiMNet::net;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop)
    : baseLoop_(baseLoop)
    , numThreads_(0)
    , next_(0) {}

void EventLoopThreadPool::start(const ThreadInitCallback& cb)
{
    for (int i = 0; i < numThreads_; ++i)
    {
        loops_.push_back(std::make_unique<EventLoop>());
    }

    for (auto& loop : loops_)
    {
        threads_.emplace_back([cb, &loop]()
        {
            if (cb)
            {
                cb(loop.get());
            }
            size_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
            loop->setThreadId(threadId);

            std::string str = "loop address: " + std::to_string(reinterpret_cast<uintptr_t>(loop.get())) + ", threadId: " + std::to_string(threadId);
            LOG_INFO(str.c_str());

            loop->loop();
        });
    }

    if (cb && numThreads_ == 0)
    {
        cb(baseLoop_);
    }
}

EventLoop* EventLoopThreadPool::getLoop()
{
    std::string str = "loop address: " + std::to_string(reinterpret_cast<uintptr_t>(baseLoop_)) + ", loop number: " + std::to_string(next_);
    LOG_INFO(str.c_str());
    EventLoop* loop = baseLoop_;
    if (!loops_.empty())
    {
        loop = loops_[next_].get();
        next_ = (next_ + 1) % loops_.size();
    }
    return loop;
}
