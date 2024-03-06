#include "InetAddress.h"

#include <arpa/inet.h>

using namespace hoshiMNet;
using namespace hoshiMNet::net;

InetAddress::InetAddress(const std::string& ip, uint16_t port)
    : ip_(ip)
    , port_(port)
{
    addr_ = {};
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
}

InetAddress::InetAddress(const struct sockaddr_in& addr)
    : ip_(inet_ntoa(addr.sin_addr))
    , port_(ntohs(addr.sin_port))
    , addr_(addr) {}

InetAddress::~InetAddress() {}