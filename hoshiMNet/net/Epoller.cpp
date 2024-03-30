#include "Epoller.h"

#include <unistd.h>

#include "../base/Log.h"
#include "Channel.h"

using namespace hoshiMNet;
using namespace hoshiMNet::net;

Epoller::Epoller(EventLoop* loop)
    : epollfd_(::epoll_create1(EPOLL_CLOEXEC))
    , loop_(loop)
    , events_(DEFAULT_EVENT_SIZE) {}

Epoller::~Epoller()
{
    ::close(epollfd_);
}

void Epoller::poll(int timeoutMs, ChannelList* activeChannels)
{
    int eventNum = ::epoll_wait(epollfd_, events_.data(), static_cast<int>(events_.size()), timeoutMs);
    if (eventNum > 0)
    {
        LOG_INFO(std::string("Epoller::poll() ") + std::to_string(eventNum) + " events happened");
        getActiveChannels(eventNum, activeChannels);
        if (static_cast<size_t>(eventNum) == events_.size())
        {
            events_.resize(events_.size() * 2);
        }
    }
    else if (eventNum == 0)
    {
        LOG_INFO("Epoller::poll() nothing happened");
    }
    else
    {
        LOG_ERROR("Epoller::poll() error");
    }
}

void Epoller::updateChannel(Channel* channel)
{
    struct epoll_event event = {};
    event.events = channel->events();
    event.data.ptr = channel;
    int fd = channel->fd();
    if (channels_.find(fd) == channels_.end())
    {
        LOG_INFO(std::string("Epoller::updateChannel() EPOLL_CTL_ADD fd = ") + std::to_string(fd));
        ::epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &event);
        channels_[fd] = channel;
    }
    else
    {
        LOG_INFO(std::string("Epoller::updateChannel() EPOLL_CTL_MOD fd = ") + std::to_string(fd));
        ::epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &event);
    }
}

void Epoller::removeChannel(Channel* channel)
{
    int fd = channel->fd();
    LOG_INFO(std::string("Epoller::removeChannel() fd = ") + std::to_string(fd));
    ::epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, 0);
    channels_.erase(fd);
}

void Epoller::getActiveChannels(int eventNum, ChannelList* activeChannels)
{
    for (int i = 0; i < eventNum; i++)
    {
        Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
        activeChannels->push_back(channel);
    }
}