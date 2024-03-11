#include "Channel.h"

#include "../base/Log.h"
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
        LOG_INFO("Channel::handleEvent() EPOLLERR");
        if (errorCallback_) errorCallback_();
    }

    if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        LOG_INFO("Channel::handleEvent() EPOLLIN | EPOLLPRI | EPOLLRDHUP");
        if (readCallback_) readCallback_();
    }

    if (revents_ & EPOLLOUT)
    {
        LOG_INFO("Channel::handleEvent() EPOLLOUT");
        if (writeCallback_) writeCallback_();
    }
}

void Channel::remove()
{
    LOG_INFO(std::string("Channel::remove() fd = ") + std::to_string(fd_));
    loop_->removeChannel(this);
}

void Channel::update()
{
    LOG_INFO(std::string("Channel::update() fd = ") + std::to_string(fd_));
    LOG_INFO(std::string("Channel::update() events = ") + std::to_string(events_));
    loop_->updateChannel(this);
}