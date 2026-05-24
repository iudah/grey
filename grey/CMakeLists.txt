add_library(grey STATIC arena.c grey_ecs.c grey_input.c grey_input.h grey_ecs.h systems/sys_render.c systems/sys_physics.c systems/sys_animator.c grey_tilemap.c systems/grey_events.c systems/grey_camera.c)
target_sources(grey PUBLIC grey_input.h grey_memory.h arena.h systems/grey_systems.h grey_tilemap.h)
target_include_directories(grey PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/systems)

target_link_libraries(grey PUBLIC raylib)
if(CMAKE_ANDROID_NDK)
  target_compile_definitions(grey PUBLIC GREY_USE_TOUCH GREY_TARGETS_ANDROID)
endif()
