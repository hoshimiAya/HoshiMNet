#include "InetAddress.h"

using namespace hoshiMNet;
using namespace hoshiMNet::net;

InetAddress::InetAddress(const std::string& ip, uint16_t port)
    : ip_(ip)
    , port_(port) {}

InetAddress::~InetAddress() {}