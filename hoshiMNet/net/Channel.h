#ifndef HOSHIMNET_NET_CHANNEL_H_
#define HOSHIMNET_NET_CHANNEL_H_

#include <sys/epoll.h>

#include <functional>

namespace hoshiMNet
{
namespace net
{

class EventLoop;

class Channel
{
public:
    using Callback = std::function<void()>;

public:
    Channel(EventLoop* loop, int fd);
    ~Channel();

    void handleEvent();
    void setReadCallback(Callback cb) { readCallback_ = cb; }
    void setWriteCallback(Callback cb) { writeCallback_ = cb; }
    void setErrorCallback(Callback cb) { errorCallback_ = cb; }
    void setCloseCallback(Callback cb) { closeCallback_ = cb; }

    int fd() const { return fd_; }
    int events() const { return events_; }
    void set_revents(int revents) { revents_ = revents; }

    void enableReading() { events_ |= POLLIN | POLLPRI; }
    void disableReading() { events_ &= ~(POLLIN | POLLPRI); }
    void enableWriting() { events_ |= POLLOUT; }
    void disableWriting() { events_ &= ~POLLOUT; }

private:
    int fd_;
    int events_;
    int revents_;
    EventLoop* loop_;

    Callback readCallback_;
    Callback writeCallback_;
    Callback errorCallback_;
    Callback closeCallback_;
};

} // namespace net
} // namespace hoshiMNet

#endif // HOSHIMNET_NET_CHANNEL_H_