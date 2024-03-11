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

    void enableReading() { events_ |= EPOLLIN | EPOLLPRI; update(); }
    void disableReading() { events_ &= ~(EPOLLIN | EPOLLPRI); update(); }
    void enableWriting() { events_ |= EPOLLOUT; update(); }
    void disableWriting() { events_ &= ~EPOLLOUT; update(); }

    void remove();

private:
    void update();

private:
    int fd_;
    EventLoop* loop_;
    int events_;
    int revents_;

    Callback readCallback_;
    Callback writeCallback_;
    Callback errorCallback_;
    Callback closeCallback_;
};

} // namespace net
} // namespace hoshiMNet

#endif // HOSHIMNET_NET_CHANNEL_H_