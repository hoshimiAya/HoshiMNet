set(SRCS
    Log.cpp
    MpmcQueue.cpp
)

add_library(hoshiMNetBase ${SRCS})

install(TARGETS hoshiMNetBase DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/hoshiMNet/base)