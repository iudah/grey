file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.c)

add_library(grey STATIC ${CMAKE_CURRENT_BINARY_DIR}/dummy.c arena.c grey_ecs.c grey_input.c)

target_link_libraries(grey PUBLIC raylib)
if(CMAKE_ANDROID_NDK)
  target_compile_definitions(grey GREY_USE_TOUCH)
endif()
