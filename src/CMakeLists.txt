# Minimum version required of CMake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
# Declaration of the project
project(
  FastFingers
  VERSION 0.6.18
  LANGUAGES C
  )
# Initializing other CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(GSettings)
include(Icons)
include(Desktop)
include(Data)
# Declaring the C standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
# Checking the presence of PkgConfig
find_package(PkgConfig REQUIRED QUIET)
# Checking the presence of GTK 4
pkg_search_module(GTK REQUIRED gtk4)
# Fetching gmodule-export-2.0
pkg_search_module(GM_EXPORT REQUIRED gmodule-export-2.0)
# Checking the presence of glib-compile-resources command and fetching it
find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)
# Defining GResources and resource files
set(GRESOURCE_C   ff_gresources.c)
set(GRESOURCE_XML org.ccextractor.FastFingers.gresource.xml)
# Defining GResources dependencies
file(GLOB GRESOURCE_DEPENDENCIES ui/* logo/* applications/* FastFingers.css)
# Defining how to compile the GResources
add_custom_command(
  OUTPUT ${GRESOURCE_C}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND ${GLIB_COMPILE_RESOURCES}
  ARGS
  --generate-source
  --target=${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
  ${GRESOURCE_XML}
  VERBATIM
  MAIN_DEPENDENCY ${GRESOURCE_XML}
  DEPENDS
  ${GRESOURCE_DEPENDENCIES}
  )

# Listing sources to link with the executable
list(APPEND ff_sources
  fastfingers.c
  ff-utils.c
  card.c
  button-box.c
  home-page.c
  application-page.c
  practice-page.c
  settings-page.c
  credits-page.c
  reset-progress-page.c
  key.c
  shortcut-list-row.c
  ${GRESOURCE_C}
  cJSON.c
  )
# Creation of the application executable
add_executable(fastfingers main.c ${ff_sources})
# Adding the GTK's include directories to the application's library
target_include_directories(fastfingers PUBLIC ${GTK_INCLUDE_DIRS} ${GM_EXPORT_INCLUDE_DIRS})
# Linking the application executable and the libraries
target_link_libraries(fastfingers ${CMAKE_THREAD_LIBS_INIT} ${GTK_LIBRARIES} ${GM_EXPORT_LIBRARIES})
# Adding necessary compile flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--export-dynamic")
# Installing the binary to the system
add_custom_command (
  TARGET fastfingers
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:fastfingers> "/usr/bin/"
  )
# Installing other files
add_schema(fastfingers)
add_icons(fastfingers)
add_desktop(fastfingers ${CMAKE_PROJECT_VERSION})
add_data(fastfingers)

