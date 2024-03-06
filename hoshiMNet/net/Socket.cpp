#include "Socket.h"

#include <sys/socket.h>
#include <unistd.h>

#include "InetAddress.h"

using namespace hoshiMNet;
using namespace hoshiMNet::net;

Socket::Socket(int fd)
    : fd_(fd) {}

Socket::~Socket()
{
    ::close(fd_);
}

void Socket::bind(const InetAddress& addr)
{
    ::bind(fd_, addr.sockAddr(), sizeof(addr.sockAddreLen()));
}

void Socket::listen()
{
    ::listen(fd_, SOMAXCONN);
}

int Socket::accept(InetAddress* peeraddr)
{
    struct sockaddr_in addr = {};
    socklen_t addrlen = sizeof(addr);
    int fd = ::accept(fd_, reinterpret_cast<struct sockaddr*>(&addr), &addrlen);
    if (fd >= 0)
    {
        peeraddr->setSockAddr(addr);
    }
    return fd;
}