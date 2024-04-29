#ifndef HOHSIMNET_NET_TCPSERVER_H_
#define HOHSIMNET_NET_TCPSERVER_H_

#include <map>
#include <memory>

#include "EventLoopThreadPool.h"
#include "InetAddress.h"
#include "TcpConnection.h"

namespace hoshiMNet
{
namespace net
{

class Acceptor;
class EventLoop;

class TcpServer
{
public:
    using ConnectMap = std::map<std::string, TcpConnectionPtr>;
    using ThreadInitCallback = std::function<void(EventLoop*)>;
    using Callback = std::function<void(const TcpConnectionPtr&)>;
    using ReadCallback = std::function<void(const TcpConnectionPtr&, const std::vector<char>&)>;

public:
    TcpServer(EventLoop* loop, const InetAddress& listenAddr);
    TcpServer(EventLoop* loop, const std::string& ip = "0.0.0.0", uint16_t port = 80);
    ~TcpServer();

    void start();

    void setThreadNum(int numThreads) { threadPool_->setThreadNum(numThreads); }
    void setThreadInitCallback(const ThreadInitCallback& cb) { threadInitCallback_ = cb; }
    void setConnectionCallback(const Callback& cb) { connectionCallback_ = cb; }
    void setMessageCallback(const ReadCallback& cb) { messageCallback_ = cb; }
    void setWriteCompleteCallback(const Callback& cb) { writeCompleteCallback_ = cb; }

private:
    void newConnection(int connfd, const InetAddress& peerAddr);
    void removeConnection(const TcpConnectionPtr& conn);
    void removeConnectionInLoop(const TcpConnectionPtr& conn);

private:
    EventLoop* loop_;
    InetAddress listenAddr_;
    std::unique_ptr<Acceptor> acceptor_;
    std::shared_ptr<EventLoopThreadPool> threadPool_;

    ConnectMap connects_;
    ThreadInitCallback threadInitCallback_;
    Callback connectionCallback_;
    ReadCallback messageCallback_;
    Callback writeCompleteCallback_;

    int connId_;
};

} // namespace net
} // namespace hoshiMNet

#endif // HOHSIMNET_NET_TCPSERVER_H_