#include "Acceptor.h"

#include <unistd.h>

#include "EventLoop.h"
#include "InetAddress.h"

using namespace hoshiMNet;
using namespace hoshiMNet::net;

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr)
    : loop_(loop)
    , socket_(Socket::create())
    , channel_(loop, socket_.fd())
{
    socket_.bind(listenAddr);
    channel_.setReadCallback(std::bind(&Acceptor::acceptNewConnection, this));
}

Acceptor::~Acceptor() {}

void Acceptor::listen()
{
    socket_.listen();
    channel_.enableReading();
}

void Acceptor::acceptNewConnection()
{
    InetAddress peerAddr;
    int connfd = socket_.accept(&peerAddr);
    if (connfd >= 0)
    {
        if (newConnectionCallback_)
        {
            newConnectionCallback_(connfd, peerAddr);
        }
        else
        {
            ::close(connfd);
        }
    }
}