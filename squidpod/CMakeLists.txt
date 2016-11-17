set(LIBSRC
    ${CMAKE_CURRENT_SOURCE_DIR}/entry.cpp
)
add_library(squidpod SHARED ${LIBSRC})
target_include_directories ( squidpod PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (squidpod LINK_PUBLIC sndfile)
