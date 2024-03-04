set(SRCS
    InetAddress.cpp
)

add_library(hoshiMNetNet ${SRCS})
target_link_libraries(hoshiMNetNet pthread)

install(TARGETS hoshiMNetNet DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/hoshiMNet/net)