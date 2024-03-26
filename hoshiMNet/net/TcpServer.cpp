#include "TcpServer.h"

#include <functional>

#include "../base/Log.h"
#include "Acceptor.h"
#include "EventLoop.h"

using namespace hoshiMNet;
using namespace hoshiMNet::net;

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr)
    : loop_(loop)
    , listenAddr_(listenAddr)
    , acceptor_(std::unique_ptr<Acceptor>(new Acceptor(loop, listenAddr)))
    , threadPool_(std::make_shared<EventLoopThreadPool>(loop))
{
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer() {}

void TcpServer::start()
{
    threadPool_->start(threadInitCallback_);
    acceptor_->listen();
}

void TcpServer::newConnection(int connfd, const InetAddress& peerAddr)
{
    std::string log = "connfd: " + std::to_string(connfd) + ", id: " + listenAddr_.ip() + ":" + std::to_string(listenAddr_.port()) + "-" + peerAddr.ip() + ":" + std::to_string(peerAddr.port());
    LOG_INFO(log.c_str());
    std::string id = listenAddr_.ip() + ":" + std::to_string(listenAddr_.port()) + "-" + peerAddr.ip() + ":" + std::to_string(peerAddr.port());
    TcpConnectionPtr conn = std::make_shared<TcpConnection>(loop_, id, connfd);
    connects_[id] = conn;

    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));

    conn->connectEstablished();
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    connects_.erase(conn->id());
    conn->connectDestroyed();
}