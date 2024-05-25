#include "TcpClient.h"

#include "../base/Log.h"
#include "EventLoop.h"

using namespace hoshiMNet;
using namespace hoshiMNet::net;

TcpClient::TcpClient(EventLoop* loop, const InetAddress& serverAddr)
    : loop_(loop)
    , socket_(Socket::create())
    , serverAddr_(serverAddr) {}

TcpClient::TcpClient(EventLoop* loop, const std::string& ip, uint16_t port)
    : TcpClient(loop, InetAddress(ip, port)) {}

TcpClient::~TcpClient() {}

void TcpClient::connect()
{
    socket_.connect(serverAddr_);
    connection_ = std::make_shared<TcpConnection>(loop_, "default", socket_.fd());

    connection_->setConnectionCallback(connectionCallback_);
    connection_->setMessageCallback(messageCallback_);
    connection_->setWriteCompleteCallback(writeCompleteCallback_);
    connection_->setCloseCallback([this](const TcpConnectionPtr& conn)
    {
        conn->connectDestroyed();
    });

    connection_->connectEstablished();
}