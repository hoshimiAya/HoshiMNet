#include "EventLoopThreadPool.h"

#include "EventLoop.h"

using namespace hoshiMNet;
using namespace hoshiMNet::net;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop)
    : baseLoop_(baseLoop) {}

void EventLoopThreadPool::start(const ThreadInitCallback& cb)
{
    if (cb)
    {
        cb(baseLoop_);
    }
}

EventLoop* EventLoopThreadPool::getLoop()
{
    return baseLoop_;
}
