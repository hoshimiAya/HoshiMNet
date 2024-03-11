
#include <chrono>
#include <condition_variable>
#include <thread>
#include <iostream>

#include "hoshiMNet/base/Log.h"
#include "hoshiMNet/base/MpmcQueue.h"
#include "hoshiMNet/base/ThreadPool.h"
#include "hoshiMNet/net/InetAddress.h"
#include "hoshiMNet/net/Socket.h"
#include "hoshiMNet/net/Acceptor.h"
#include "hoshiMNet/net/EventLoop.h"
#include "hoshiMNet/net/Channel.h"
#include "hoshiMNet/net/Epoller.h"

void testLog()
{
    hoshiMNet::Log::instance().log("Hello, spdlog!");
    LOG_INFO("Hello, spdlog!");
    LOG_WARN("Hello, spdlog!");
    LOG_ERROR("Hello, spdlog!");
}

void testMpmcQueue()
{
    hoshiMNet::base::MpmcQueue<int> queue(10);

    for (int i = 0; i < 12; ++i)
    {
        std::cout << "size: " << queue.size() << ", i: " << i << std::endl;
        queue.push(std::move(i));
    }

    std::cout << "size: " << queue.size() << std::endl;
    std::cout << "maxSize: " << queue.maxSize() << std::endl;

    int value;
    for (int i = 0; i < 12; ++i)
    {
        queue.pop(value);
        std::cout << value << std::endl;
    }
}

void testThreadPool()
{
    hoshiMNet::base::ThreadPool pool(4, 10);
    pool.start();

    std::function<void(int)> task = [](int i)
    {
        // sleep 1s
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // print thread
        std::string str = "thread id: " + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())) + ", task: " + std::to_string(i);
        LOG_INFO(str);
    };

    for (int i = 0; i < 100; ++i)
    {
        pool.post(std::bind(task, i));
        std::string str = "post: " + std::to_string(i);
        LOG_INFO(str);
    }

    pool.stop();
    // pool.waitDone();
}

void testInetAddress()
{
    hoshiMNet::net::InetAddress addr("127.0.0.1", 8080);
    std::string ip = addr.ip();
    uint16_t port = addr.port();
    std::string str = "ip: " + ip + ", port: " + std::to_string(port);
    LOG_INFO(str);

    struct sockaddr_in sockAddr;
    hoshiMNet::net::InetAddress addr2(sockAddr);
    std::string str2 = "ip: " + addr2.ip() + ", port: " + std::to_string(addr2.port());
    LOG_INFO(str2);

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(8081);
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.setSockAddr(sockAddr);
    addr2.setSockAddr(sockAddr);
    std::string str3 = "ip: " + addr.ip() + ", port: " + std::to_string(addr.port());
    LOG_INFO(str3);
    std::string str4 = "ip: " + addr2.ip() + ", port: " + std::to_string(addr2.port());
    LOG_INFO(str4);
}

void testSocket()
{
    hoshiMNet::net::Socket socket = hoshiMNet::net::Socket::create();
    LOG_INFO(std::string("socket fd: ") + std::to_string(socket.fd()));

    hoshiMNet::net::InetAddress addr("0.0.0.0", 17151);
    socket.bind(addr);
    LOG_INFO(std::string("ip: ") + addr.ip() + ", port: " + std::to_string(addr.port()));

    socket.listen();

    hoshiMNet::net::InetAddress peeraddr;
    int fd = socket.accept(&peeraddr);
    LOG_INFO(std::string("peer fd: ") + std::to_string(fd));
    LOG_INFO(std::string("ip: ") + peeraddr.ip() + ", port: " + std::to_string(peeraddr.port()));
}

void testAcceptor()
{
    hoshiMNet::net::EventLoop loop;
    hoshiMNet::net::InetAddress addr("0.0.0.0", 17151);

    // socket绑定、监听，channel设置回调
    hoshiMNet::net::Acceptor acceptor(&loop, addr);

    acceptor.setNewConnectionCallback([](int sockfd, const hoshiMNet::net::InetAddress& tempAddr)
    {
        LOG_INFO(std::string("new connection: ") + std::to_string(sockfd));
        LOG_INFO(std::string("ip: ") + tempAddr.ip() + ", port: " + std::to_string(tempAddr.port()));
    });
    acceptor.listen();

    loop.loop();

    LOG_INFO("loop exit");
}

int main()
{
    // testLog();
    // testMpmcQueue();
    // testThreadPool();
    // testInetAddress();
    // testSocket();
    testAcceptor();
    return 0;
}