#ifndef HOSHIMNET_NET_INETADDRESS_H_
#define HOSHIMNET_NET_INETADDRESS_H_

#include <sys/socket.h>
#include <netinet/in.h>

#include <string>

namespace hoshiMNet
{
namespace net
{

class InetAddress
{
public:
    InetAddress() = default;
    ~InetAddress() = default;
    InetAddress(const std::string& ip, uint16_t port);
    explicit InetAddress(const struct sockaddr_in& addr);

    void setSockAddr(const struct sockaddr_in& addr);
    std::string ip() const { return ip_; }
    uint16_t port() const { return port_; }
    const struct sockaddr* sockAddr() const { return reinterpret_cast<const struct sockaddr*>(&addr_); }
    int sockAddreLen() const { return sizeof(struct sockaddr_in); }

private:
    std::string ip_;
    uint16_t port_;
    struct sockaddr_in addr_;
};

} // namespace net
} // namespace hoshiMNet

#endif // HOSHIMNET_NET_INETADDRESS_H_