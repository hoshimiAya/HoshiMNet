#ifndef HOSHIMNET_NET_EPOLLER_H_
#define HOSHIMNET_NET_EPOLLER_H_

#include <sys/epoll.h>

#include <map>
#include <vector>

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
    using ChannelList = std::vector<Channel*>;
    using EventList = std::vector<struct epoll_event>;

public:
    Epoller(EventLoop* loop);
    ~Epoller();

    void poll(int timeoutMs, ChannelList* activeChannels);
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);

private:
    void getActiveChannels(int eventNum, ChannelList* activeChannels);

private:
    int epollfd_;
    EventLoop* loop_;
    ChannelMap channels_;
    EventList events_;

    static const int DEFAULT_EVENT_SIZE = 16;
};

} // namespace net
} // namespace hoshiMNet

#endif // HOSHIMNET_NET_EPOLLER_H_