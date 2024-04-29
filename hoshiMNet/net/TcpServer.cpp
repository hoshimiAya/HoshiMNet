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
    , connId_(0)
{
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::TcpServer(EventLoop* loop, const std::string& ip, uint16_t port)
    : TcpServer(loop, InetAddress(ip, port)) {}

TcpServer::~TcpServer() {}

void TcpServer::start()
{
    threadPool_->start(threadInitCallback_);
    loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
}

void TcpServer::newConnection(int connfd, const InetAddress& peerAddr)
{
    std::string log = "connfd: " + std::to_string(connfd) + ", id: " + listenAddr_.ip() + ":" + std::to_string(listenAddr_.port()) + "-" + peerAddr.ip() + ":" + std::to_string(peerAddr.port());
    LOG_INFO(log.c_str());

    EventLoop* loop = threadPool_->getLoop();
    std::string id = peerAddr.ip() + ":" + std::to_string(peerAddr.port()) + "-" + std::to_string(++connId_);
    TcpConnectionPtr conn = std::make_shared<TcpConnection>(loop, id, connfd);

    std::string str = "new connection: " + conn->id();
    LOG_INFO(str.c_str());
    connects_[id] = conn;

    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnectionInLoop, this, std::placeholders::_1));

    loop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    std::string str = "remove connection: " + conn->id();
    LOG_INFO(str.c_str());
    connects_.erase(conn->id());
    EventLoop* loop = conn->getLoop();
    loop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
    loop_->runInLoop(std::bind(&TcpServer::removeConnection, this, conn));
}