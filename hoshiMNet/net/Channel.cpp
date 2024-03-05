#include "Channel.h"

#include "EventLoop.h"

using namespace hoshiMNet;
using namespace hoshiMNet::net;

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop),
      fd_(fd),
      events_(0),
      revents_(0) {}

Channel::~Channel() {}

void Channel::handleEvent()
{
    if (revents_ & POLLNVAL)
    {
        // LOG_WARN << "Channel::handle_event() POLLNVAL";
    }

    if (revents_ & (POLLERR | POLLNVAL))
    {
        if (errorCallback_) errorCallback_();
    }

    if (revents_ & (POLLIN | POLLPRI | POLLRDHUP))
    {
        if (readCallback_) readCallback_();
    }

    if (revents_ & POLLOUT)
    {
        if (writeCallback_) writeCallback_();
    }
}