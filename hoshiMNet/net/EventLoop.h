#ifndef HOSHIMNET_NET_EVENTLOOP_H_
#define HOSHIMNET_NET_EVENTLOOP_H_

#include <memory>
#include <vector>

namespace hoshiMNet
{
namespace net
{

class Channel;
class Epoller;

class EventLoop
{
public:
    using ChannelList = std::vector<Channel*>;

public:
    EventLoop();
    ~EventLoop();

    void loop();

private:
    bool stop_;
    int pollTimeout_;
    std::unique_ptr<Epoller> epoller_;
    ChannelList activeChannels_;

};

} // namespace net
} // namespace hoshiMNet

#endif // HOSHIMNET_NET_EVENTLOOP_H_