#include "TcpConnection.h"

#include "../base/Log.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

using namespace hoshiMNet;
using namespace hoshiMNet::net;

TcpConnection::TcpConnection(EventLoop* loop, const std::string& id, int connfd)
    : socket_(std::unique_ptr<Socket>(new Socket(connfd)))
    , channel_(std::unique_ptr<Channel>(new Channel(loop, connfd)))
    , loop_(loop)
    , id_(id)
{
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));
}

TcpConnection::~TcpConnection() {}

void TcpConnection::connectEstablished()
{
    LOG_INFO("TcpConnection::connectEstablished\n");
    channel_->enableReading();

    if (connectionCallback_)
    {
        connectionCallback_(shared_from_this());
    }
}

void TcpConnection::connectDestroyed()
{
    LOG_INFO("TcpConnection::connectDestroyed\n");
    channel_->disableAll();
    channel_->remove();
}

void TcpConnection::handleRead()
{
    LOG_INFO("TcpConnection::handleRead\n");

    std::vector<char> buf(4096); // 4KB // temp
    size_t n = socket_->read(buf.data(), buf.size());

    if (n > 0)
    {
        std::string str = "recv: " + std::string(buf.data());
        LOG_INFO(str.c_str());

        if (messageCallback_)
        {
            messageCallback_(shared_from_this(), buf);
        }
    }
    else if (n == 0)
    {
        handleClose();
    }
    else
    {
        handleError();
    }
}

void TcpConnection::handleWrite()
{
    LOG_INFO("TcpConnection::handleWrite\n");

    if (writeCompleteCallback_)
    {
        writeCompleteCallback_(shared_from_this());
    }
}

void TcpConnection::handleClose()
{
    LOG_INFO("TcpConnection::handleClose\n");
    channel_->disableAll();
    if (closeCallback_)
    {
        closeCallback_(shared_from_this());
    }
}

void TcpConnection::handleError()
{
    LOG_ERROR("TcpConnection::handleRead\n");
}