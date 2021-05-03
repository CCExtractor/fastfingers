cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(FastFingers LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED QUIET)

pkg_search_module(GTK REQUIRED gtk4)

add_executable(ff_app main.c)

add_library(ff_library
  SHARED
  ff_application.c
  ff_application.h
  ff_utils.c
  ff_utils.h
  ff_cards.c
  ff_cards.h
  ff_home.c
  ff_home.h
  ff_practice.c
  ff_practice.h
  )

configure_file(${CMAKE_SOURCE_DIR}/ff_style.css ${CMAKE_BINARY_DIR}/ff_style.css COPYONLY)
file(COPY ${CMAKE_SOURCE_DIR}/logos DESTINATION ${CMAKE_BINARY_DIR})

target_include_directories(ff_library PUBLIC ${GTK_INCLUDE_DIRS})

target_link_libraries(ff_app ff_library ${CMAKE_THREAD_LIBS_INIT} ${GTK_LIBRARIES})
