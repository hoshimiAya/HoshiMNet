#include "Channel.h"

#include "EventLoop.h"

using namespace hoshiMNet;
using namespace hoshiMNet::net;

Channel::Channel(EventLoop* loop, int fd)
    : fd_(fd)
    , loop_(loop)
    , events_(0)
    , revents_(0) {}

Channel::~Channel() {}

void Channel::handleEvent()
{
    if (revents_ & EPOLLERR)
    {
        if (errorCallback_) errorCallback_();
    }

    if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        if (readCallback_) readCallback_();
    }

    if (revents_ & EPOLLOUT)
    {
        if (writeCallback_) writeCallback_();
    }
}