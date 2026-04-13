cmake_minimum_required(VERSION 3.20)
project(GreyWorkspace C)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

set(CMAKE_C_STANDARD 23)

if(CMAKE_ANDROID_NDK)
  message(STATUS "Configuring Raylib for Android")
  set(PLATFORM "Android" CACHE STRING "Raylib Platform" FORCE)
else()
  message(STATUS "Configuring Raylib for Desktop")
  if(ANDROID)
    message(STATUS "Use termux glfw to work around missing joystick")
    set(USE_EXTERNAL_GLFW ON CACHE BOOL "" FORCE)
  endif()
  set(PLATFORM "Desktop" CACHE STRING "Raylib Platform" FORCE)
endif()
add_subdirectory(external/raylib)
add_subdirectory(grey) # the engine


set(ACTIVE_GAME "greg" CACHE STRING "The game project to compile")
message(STATUS "Active game: ${ACTIVE_GAME}")

add_subdirectory(${ACTIVE_GAME})

