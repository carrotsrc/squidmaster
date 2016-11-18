set(LIBSRC
    ${CMAKE_CURRENT_SOURCE_DIR}/entry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/client.cpp
)
add_library(squidjack SHARED ${LIBSRC})
target_include_directories ( squidjack PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_include_directories ( squidjack PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (squidjack LINK_PUBLIC squidpod jack pthread)

