set(GAME_SOURCES main.c)

if(CMAKE_ANDROID_NDK)
  message(STATUS "Building game as Android shared library")

  add_library(greg SHARED ${GAME_SOURCES})

  target_link_libraries(greg PRIVATE grey android log EGL GLESv2)
  target_link_options(greg PUBLIC -u ANativeActivity_onCreate)
else()
  message(STATUS "Building game as Desktop executable")

  add_executable(greg ${GAME_SOURCES})

  target_link_libraries(greg PRIVATE grey)
  
  install(TARGETS greg DESTINATION bin)
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION assets)
endif()

include(CPack)