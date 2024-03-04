#ifndef HOSHIMNET_NET_INETADDRESS_H_
#define HOSHIMNET_NET_INETADDRESS_H_

#include <string>

namespace hoshiMNet
{
namespace net
{

class InetAddress
{
public:
    InetAddress(const std::string& ip, uint16_t port);
    ~InetAddress();

    std::string ip() const { return ip_; }
    uint16_t port() const { return port_; }

private:
    std::string ip_;
    uint16_t port_;
};

} // namespace net
} // namespace hoshiMNet

#endif // HOSHIMNET_NET_INETADDRESS_H_