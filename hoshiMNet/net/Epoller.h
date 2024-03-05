#ifndef HOSHIMNET_NET_EPOLLER_H_
#define HOSHIMNET_NET_EPOLLER_H_

#include <sys/epoll.h>

#include <map>

namespace hoshiMNet
{
namespace net
{

class Channel;
class EventLoop;

class Epoller
{
public:
    using ChannelMap = std::map<int, Channel*>;

private:
    int epollfd_;
    ChannelMap channels_;
    EventLoop* loop_;
    
};

} // namespace net
} // namespace hoshiMNet

#endif // HOSHIMNET_NET_EPOLLER_H_