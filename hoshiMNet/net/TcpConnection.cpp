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
    inputBuffer_.reserve(DEFAULT_BUFFER_SIZE);
    inputBuffer_.resize(DEFAULT_BUFFER_SIZE);
    outputBuffer_.reserve(DEFAULT_BUFFER_SIZE);

    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));
}

TcpConnection::~TcpConnection() {}

void TcpConnection::send(const std::vector<char>& message)
{
    LOG_INFO("TcpConnection::send\n");
    ssize_t sentSize = 0;
    size_t remaining = message.size();

    if (outputBuffer_.empty())
    {
        sentSize = socket_->write(message.data(), message.size());
        if (sentSize < 0)
        {
            LOG_ERROR("TcpConnection::send error\n");
            return;
        }

        std::string str = "send: " + std::string(message.data());
        LOG_INFO(str.c_str());
        remaining -= sentSize;
    }

    if (remaining > 0)
    {
        outputBuffer_.insert(outputBuffer_.end(), message.begin() + sentSize, message.end());
        channel_->enableWriting();
    }
    else if (remaining == 0 && writeCompleteCallback_)
    {
        writeCompleteCallback_(shared_from_this());
    }
}

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

    ssize_t readSize = socket_->read(inputBuffer_.data(), inputBuffer_.size());

    if (readSize > 0)
    {
        std::vector<char> data(inputBuffer_.begin(), inputBuffer_.begin() + readSize);
        if (messageCallback_)
        {
            messageCallback_(shared_from_this(), data);
        }
    }
    else if (readSize == 0)
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

    if (!outputBuffer_.empty())
    {
        ssize_t sentSize = socket_->write(outputBuffer_.data(), outputBuffer_.size());
        if (sentSize < 0)
        {
            LOG_ERROR("TcpConnection::handleWrite error\n");
            return;
        }
        else if (sentSize == 0)
        {
            return;
        }

        outputBuffer_.erase(outputBuffer_.begin(), outputBuffer_.begin() + sentSize);
        if (outputBuffer_.empty())
        {
            channel_->disableWriting();
            if (writeCompleteCallback_)
            {
                writeCompleteCallback_(shared_from_this());
            }
        }
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
    LOG_ERROR("TcpConnection::handleError\n");
}