#ifndef HOSHIMNET_NET_TCPCLIENT_H_
#define HOSHIMNET_NET_TCPCLIENT_H_

#include "InetAddress.h"
#include "Socket.h"
#include "TcpConnection.h"

namespace hoshiMNet
{
namespace net
{

class EventLoop;

class TcpClient
{
public:
    using Callback = std::function<void(const TcpConnectionPtr&)>;
    using ReadCallback = std::function<void(const TcpConnectionPtr&, const std::vector<char>&)>;

public:
    TcpClient(EventLoop* loop, const InetAddress& serverAddr);
    TcpClient(EventLoop* loop, const std::string& ip, uint16_t port = 80);
    ~TcpClient();

    void connect();

    void setConnectionCallback(const Callback& cb) { connectionCallback_ = cb; }
    void setMessageCallback(const ReadCallback& cb) { messageCallback_ = cb; }
    void setWriteCompleteCallback(const Callback& cb) { writeCompleteCallback_ = cb; }

private:
    EventLoop* loop_;
    Socket socket_;
    InetAddress serverAddr_;
    TcpConnectionPtr connection_;

    Callback connectionCallback_;
    ReadCallback messageCallback_;
    Callback writeCompleteCallback_;
};

} // namespace net
} // namespace hoshiMNet

#endif // HOSHIMNET_NET_TCPCLIENT_H_