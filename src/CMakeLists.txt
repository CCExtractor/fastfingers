# Minimum version required of CMake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
# Declaration of the project
project(
  FastFingers
  VERSION 0.5.21
  LANGUAGES C
  )
# Initializing other CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include (GSettings)
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
file(GLOB GRESOURCE_DEPENDENCIES ui/* logo/* FastFingers.css)
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
set(_d_version ${CMAKE_PROJECT_VERSION})
configure_file(
  "${CMAKE_SOURCE_DIR}/../data/org.ccextractor.FastFingers.desktop.in"
  "/usr/share/applications/org.ccextractor.FastFingers.desktop"
  @ONLY)

list(APPEND HICOLOR_PX 16 32 48 64 128)
foreach (PX IN LISTS HICOLOR_PX)
  add_custom_command (
    TARGET fastfingers
    COMMAND
    ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/../data/icons/hicolor/${PX}x${PX}/fastfingers.png"
    "/usr/share/icons/hicolor/${PX}x${PX}/apps/"
    )
endforeach()
execute_process(COMMAND gtk-update-icon-cache -t -f /usr/share/icons/hicolor/)

list(APPEND HICONTRAST_PX 16 22 24 32 48 256)
foreach (PX IN LISTS HICONTRAST_PX)
  add_custom_command (
    TARGET fastfingers
    COMMAND
    ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/../data/icons/HighContrast/${PX}x${PX}/fastfingers.png"
    "/usr/share/icons/HighContrast/${PX}x${PX}/apps/"
    )
endforeach()
execute_process(COMMAND gtk-update-icon-cache -t -f /usr/share/icons/HighContrast/)


