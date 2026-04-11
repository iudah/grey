file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.c)

add_library(grey STATIC ${CMAKE_CURRENT_BINARY_DIR}/dummy.c arena.c)

target_link_libraries(grey PUBLIC raylib)

