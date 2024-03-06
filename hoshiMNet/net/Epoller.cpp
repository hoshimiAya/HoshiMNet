#include "Epoller.h"

#include <unistd.h>

#include "Channel.h"

using namespace hoshiMNet;
using namespace hoshiMNet::net;

Epoller::Epoller(EventLoop* loop)
    : epollfd_(::epoll_create1(EPOLL_CLOEXEC))
    , loop_(loop) {}

Epoller::~Epoller()
{
    ::close(epollfd_);
}

void Epoller::poll(int timeoutMs, ChannelList* activeChannels)
{
    int eventNum = ::epoll_wait(epollfd_, events_.data(), static_cast<int>(events_.size()), timeoutMs);
    if (eventNum > 0)
    {
        getActiveChannels(eventNum, activeChannels);
    }
    else if (eventNum == 0)
    {
        // nothing happened
    }
    else
    {
        // error
    }
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