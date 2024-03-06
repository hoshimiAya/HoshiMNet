set(SRCS
    Acceptor.cpp
    Channel.cpp
    Epoller.cpp
    EventLoop.cpp
    InetAddress.cpp
    Socket.cpp
)

add_library(hoshiMNetNet ${SRCS})
target_link_libraries(hoshiMNetNet pthread)

install(TARGETS hoshiMNetNet DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/hoshiMNet/net)