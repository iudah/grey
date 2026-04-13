add_library(grey STATIC arena.c grey_ecs.c grey_input.c grey_input.h grey_ecs.h)
target_sources(grey PUBLIC grey_input.h grey_memory.h arena.h)
target_include_directories(grey PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(grey PUBLIC raylib)
if(CMAKE_ANDROID_NDK)
  target_compile_definitions(grey PRIVATE GREY_USE_TOUCH)
endif()
