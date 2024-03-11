#ifndef HOSHIMNET_NET_SOCKET_H_
#define HOSHIMNET_NET_SOCKET_H_

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

    int fd() const { return fd_; }

private:
    int fd_;

};

} // namespace net
} // namespace hoshiMNet

#endif // HOSHIMNET_NET_SOCKET_H_