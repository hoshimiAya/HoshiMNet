set(SRCS
    Log.cpp
)

add_library(hoshiMNetBase ${SRCS})

install(TARGETS hoshiMNetBase DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/hoshiMNet/base)