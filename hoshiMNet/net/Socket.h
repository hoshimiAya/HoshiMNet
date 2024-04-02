#ifndef HOSHIMNET_NET_SOCKET_H_
#define HOSHIMNET_NET_SOCKET_H_

#include <cstddef>

namespace hoshiMNet
{
namespace net
{

class InetAddress;

class Socket
{
public:
    explicit Socket(int fd);
    ~Socket();

    static Socket create();
    void bind(const InetAddress& addr);
    void listen();
    int accept(InetAddress* peeraddr);
    int connect(const InetAddress& addr);
    size_t read(void* buf, size_t count);
    size_t write(const void* buf, size_t count);

    int fd() const { return fd_; }

private:
    int fd_;

};

} // namespace net
} // namespace hoshiMNet

#endif // HOSHIMNET_NET_SOCKET_H_