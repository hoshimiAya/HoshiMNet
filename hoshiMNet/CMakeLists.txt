set(SRCS
    temp.cpp
)

add_library(hoshiMNet ${SRCS})
# target_link_libraries(hoshiMNet pthread)

install(TARGETS hoshiMNet DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/hoshiMNet)