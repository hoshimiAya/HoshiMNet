#include "EventLoop.h"

#include <sys/eventfd.h>
#include <unistd.h>

#include <thread>

#include "../base/Log.h"
#include "Channel.h"
#include "Epoller.h"

using namespace hoshiMNet;
using namespace hoshiMNet::net;

EventLoop::EventLoop()
    : stop_(false)
    , pollTimeout_(1000 * 10)
    , epoller_(std::make_unique<Epoller>(this))
    , wakeupFd_(::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC))
    , wakeupChannel_(std::make_unique<Channel>(this, wakeupFd_))
    , callingPendingFunctors_(false)
    , threadId_(0)
{
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    ::close(wakeupFd_);
}

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

        doPendingFunctors();
    }
}

void EventLoop::updateChannel(Channel* channel)
{
    epoller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
    epoller_->removeChannel(channel);
}

void EventLoop::runInLoop(const Functor& cb)
{
    if (isInLoopThread())
    {
        cb();
    }
    else
    {
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(const Functor& cb)
{
    std::unique_lock<std::mutex> lock(mutex_);
    pendingFunctors_.push_back(cb);
    lock.unlock();

    if (!isInLoopThread() || callingPendingFunctors_)
    {
        wakeup();
    }
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;

    std::unique_lock<std::mutex> lock(mutex_);
    functors.swap(pendingFunctors_);
    lock.unlock();

    for (const auto& functor : functors)
    {
        functor();
    }

    callingPendingFunctors_ = false;
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = ::write(wakeupFd_, &one, sizeof(one));
    if (n != sizeof one)
    {
        LOG_ERROR("EventLoop::wakeup() writes " + std::to_string(n) + " bytes instead of 8");
    }
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = ::read(wakeupFd_, &one, sizeof(one));
    if (n != sizeof one)
    {
        LOG_ERROR("EventLoop::handleRead() reads " + std::to_string(n) + " bytes instead of 8");
    }
}