#ifndef HOSHIMNET_NET_EVENTLOOP_H_
#define HOSHIMNET_NET_EVENTLOOP_H_

#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace hoshiMNet
{
namespace net
{

class Channel;
class Epoller;

class EventLoop
{
public:
    using ChannelList = std::vector<Channel*>;
    using Functor = std::function<void()>;

public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);

    bool isInLoopThread() const { return threadId_ == std::hash<std::thread::id>{}(std::this_thread::get_id()); }
    void runInLoop(const Functor& cb);
    void queueInLoop(const Functor& cb);
    void doPendingFunctors();

    void setThreadId(size_t threadId) { threadId_ = threadId; }
    size_t threadId() const { return threadId_; }

private:
    void wakeup();
    void handleRead();

private:
    bool stop_;
    int pollTimeout_;
    std::unique_ptr<Epoller> epoller_;
    ChannelList activeChannels_;

    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;

    std::mutex mutex_;
    std::vector<Functor> pendingFunctors_;
    bool callingPendingFunctors_;
    size_t threadId_;
};

} // namespace net
} // namespace hoshiMNet

#endif // HOSHIMNET_NET_EVENTLOOP_H_