#ifndef HOSHIMNET_NET_ACCEPTOR_H_
#define HOSHIMNET_NET_ACCEPTOR_H_

#include "Channel.h"
#include "Socket.h"

namespace hoshiMNet
{
namespace net
{

class EventLoop;

class Acceptor
{
public:
    using NewConnectionCallback = std::function<void(int sockfd, const InetAddress&)>;

public:
    Acceptor(EventLoop* loop, const InetAddress& listenAddr);
    ~Acceptor();

    void listen();

    void setNewConnectionCallback(const NewConnectionCallback& cb) { newConnectionCallback_ = cb; }

private:
    void acceptNewConnection();

private:
    EventLoop* loop_;
    Socket socket_;
    Channel channel_;
    NewConnectionCallback newConnectionCallback_;
};

} // namespace net
} // namespace hoshiMNet

#endif // HOSHIMNET_NET_ACCEPTOR_H_