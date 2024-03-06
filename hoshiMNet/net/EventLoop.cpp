#include "EventLoop.h"

#include "Channel.h"
#include "Epoller.h"

using namespace hoshiMNet;
using namespace hoshiMNet::net;

EventLoop::EventLoop()
    : stop_(false)
    , pollTimeout_(1000 * 10)
    , epoller_(std::make_unique<Epoller>(this)) {}

EventLoop::~EventLoop() {}

void EventLoop::loop()
{
    while (!stop_)
    {
        activeChannels_.clear();
        epoller_->poll(pollTimeout_, &activeChannels_);
        for (auto channel : activeChannels_)
        {
            channel->handleEvent();
        }
    }
}