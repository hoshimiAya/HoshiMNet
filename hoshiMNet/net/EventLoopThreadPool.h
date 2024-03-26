#ifndef HOSHIMNET_NET_EVENTLOOPTHREADPOOL_H_
#define HOSHIMNET_NET_EVENTLOOPTHREADPOOL_H_

#include <functional>

namespace hoshiMNet
{
namespace net
{

class EventLoop;

class EventLoopThreadPool // only main loop and main thread now
{
public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;

public:
    explicit EventLoopThreadPool(EventLoop* baseLoop);
    ~EventLoopThreadPool() = default;

    void start(const ThreadInitCallback& cb);
    EventLoop* getLoop();

private:
    EventLoop* baseLoop_;
};

} // namespace net
} // namespace hoshiMNet

#endif // HOSHIMNET_NET_EVENTLOOPTHREADPOOL_H_