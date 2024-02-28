set(SRCS
    Log.cpp
    ThreadPool.cpp
)

add_library(hoshiMNetBase ${SRCS})
target_link_libraries(hoshiMNetBase pthread)

install(TARGETS hoshiMNetBase DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/hoshiMNet/base)