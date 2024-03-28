#ifndef HOSHIMNET_NET_TCPCONNECTION_H_
#define HOSHIMNET_NET_TCPCONNECTION_H_

#include <memory>
#include <vector>

namespace hoshiMNet
{
namespace net
{

class Channel;
class EventLoop;
class Socket;
class TcpConnection;

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
    using Callback = std::function<void(const TcpConnectionPtr&)>;
    using ReadCallback = std::function<void(const TcpConnectionPtr&, const std::vector<char>&)>;

public:
    TcpConnection(EventLoop* loop, const std::string& id, int connfd);
    ~TcpConnection();

    void send(const std::vector<char>& message);
    void connectEstablished();
    void connectDestroyed();

    void setConnectionCallback(const Callback& cb) { connectionCallback_ = cb; }
    void setMessageCallback(const ReadCallback& cb) { messageCallback_ = cb; }
    void setWriteCompleteCallback(const Callback& cb) { writeCompleteCallback_ = cb; }
    void setCloseCallback(const Callback& cb) { closeCallback_ = cb; }

    const std::string& id() const { return id_; }

private:
    void handleRead();
    void handleWrite();
    void handleClose();
    void handleError();

private:
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    EventLoop* loop_;
    std::string id_;

    std::vector<char> inputBuffer_;
    std::vector<char> outputBuffer_;
    static const int DEFAULT_BUFFER_SIZE = 1024 * 1024;

    Callback connectionCallback_;
    ReadCallback messageCallback_;
    Callback writeCompleteCallback_;
    Callback closeCallback_;
};

} // namespace net
} // namespace hoshiMNet

#endif // HOSHIMNET_NET_TCPCONNECTION_H_