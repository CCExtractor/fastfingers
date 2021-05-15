# Minimum version required of CMake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
# Declaration of the project
project(FastFingers LANGUAGES C)
# Declaring the C standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
# Checking the presence of PkgConfig
find_package(PkgConfig REQUIRED QUIET)
# Checking the presence of GTK 4
pkg_search_module(GTK REQUIRED gtk4)
# Checking the presence of glib-compile-resources command and fetching it
find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)
# Defining GResources and resource files
set(GRESOURCE_C   ff_gresources.c)
set(GRESOURCE_XML org.fastfingers.FastFingers.gresource.xml)
# Defining how to compile the GResources and the dependencies
file(GLOB GRESOURCE_DEPENDENCIES ui/* logo/* FastFingers.css)
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
  key.c
  ${GRESOURCE_C}
  cJSON.c
  )
# Creation of the application executable
add_executable(ff_app main.c ${ff_sources})
# Adding the GTK's include directories to the application's library
target_include_directories(ff_app PUBLIC ${GTK_INCLUDE_DIRS})
# Linking the application executable and the libraries
target_link_libraries(ff_app ${CMAKE_THREAD_LIBS_INIT} ${GTK_LIBRARIES})
