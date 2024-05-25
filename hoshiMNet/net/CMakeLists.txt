set(SRCS
    Acceptor.cpp
    Channel.cpp
    Epoller.cpp
    EventLoop.cpp
    EventLoopThreadPool.cpp
    InetAddress.cpp
    Socket.cpp
    TcpClient.cpp
    TcpConnection.cpp
    TcpServer.cpp
)

add_library(hoshiMNetNet ${SRCS})
target_link_libraries(hoshiMNetNet pthread)
target_link_libraries(hoshiMNetNet hoshiMNetBase)

install(TARGETS hoshiMNetNet DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/hoshiMNet/net)