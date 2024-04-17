#include "Socket.h"

#include <sys/socket.h>
#include <unistd.h>

#include <stdexcept>

#include "InetAddress.h"

using namespace hoshiMNet;
using namespace hoshiMNet::net;

Socket::Socket(int fd)
    : fd_(fd) {}

Socket::~Socket()
{
    ::close(fd_);
}

Socket Socket::create()
{
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        throw std::runtime_error("socket error");
    }
    return Socket(fd);
}

void Socket::bind(const InetAddress& addr)
{
    if (::bind(fd_, addr.sockAddr(), addr.sockAddreLen()) < 0)
    {
        ::close(fd_);
        throw std::runtime_error("bind error");
    }
}

void Socket::listen()
{
    if (::listen(fd_, SOMAXCONN) < 0)
    {
        ::close(fd_);
        throw std::runtime_error("listen error");
    }
}

int Socket::accept(InetAddress* peeraddr)
{
    struct sockaddr_in addr = {};
    socklen_t addrlen = sizeof(addr);
    int connectFd = ::accept(fd_, reinterpret_cast<struct sockaddr*>(&addr), &addrlen);
    if (connectFd < 0)
    {
        ::close(fd_);
        throw std::runtime_error("accept error");
    }
    if (peeraddr)
    {
        peeraddr->setSockAddr(addr);
    }
    return connectFd;
}

int Socket::connect(const InetAddress& addr)
{
    return ::connect(fd_, addr.sockAddr(), addr.sockAddreLen());
}

size_t Socket::read(void* buf, size_t count)
{
    return ::read(fd_, buf, count);
}

size_t Socket::write(const void* buf, size_t count)
{
    return ::write(fd_, buf, count);
}

void Socket::shutdownWrite()
{
    if (::shutdown(fd_, SHUT_WR) < 0)
    {
        throw std::runtime_error("shutdown error");
    }
}