# Minimum version required of CMake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
# Declaration of the project
file(READ ../.version VERSION)
string(STRIP ${VERSION} VERSION)
add_definitions( -DVERSION="${VERSION}" )
project(
        FastFingers
        VERSION ${VERSION}
        LANGUAGES C)
# Initializing other CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(Desktop)
# Declaring the C standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
# Checking the presence of PkgConfig
find_package(PkgConfig REQUIRED QUIET)
# Checking the presence of GTK 4
pkg_search_module(GTK REQUIRED gtk+-3.0)
# Fetching gmodule-export-2.0
pkg_search_module(GM_EXPORT REQUIRED gmodule-export-2.0)
# Checking the presence of glib-compile-resources command and fetching it
find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)
# Defining GResources and resource files
set(GRESOURCE_C ff_gresources.c)
set(GRESOURCE_XML org.ccextractor.FastFingers.gresource.xml)
# Defining GResources dependencies
file(GLOB GRESOURCE_DEPENDENCIES ui/* logo/* FastFingers.css)
# Defining how to compile the GResources
add_custom_command(
        OUTPUT ${GRESOURCE_C}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${GLIB_COMPILE_RESOURCES} ARGS --generate-source
        --target=${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C} ${GRESOURCE_XML}
        VERBATIM
        MAIN_DEPENDENCY ${GRESOURCE_XML}
        DEPENDS ${GRESOURCE_DEPENDENCIES})
# Listing sources to link with the executable
list(
        APPEND
        FASTFINGERS_SOURCES
        main.c
        fastfingers.c
        ff-utils.c
        card.c
        button-box.c
        home-page.c
        application-page.c
        practice-page.c
        quiz-page.c
        quiz-result-page.c
        settings-page.c
        credits-page.c
        reset-progress-page.c
        key.c
        shortcut-list-row.c
        cJSON.c
        ${GRESOURCE_C})

list(
        APPEND
        CHEATSHEET_SOURCES
        cheatsheet.c
        ff-utils.c
        cJSON.c
        active-window.c
        ${GRESOURCE_C})

# Creation of the application executable
add_executable(fastfingers ${FASTFINGERS_SOURCES})
add_executable(cheatsheet ${CHEATSHEET_SOURCES})
# Adding the GTK's include directories to the application's library
target_include_directories(fastfingers PUBLIC ${GTK_INCLUDE_DIRS}
        ${GM_EXPORT_INCLUDE_DIRS})
target_include_directories(cheatsheet PUBLIC ${GTK_INCLUDE_DIRS}
        ${GM_EXPORT_INCLUDE_DIRS})
# Linking the application executable and the libraries
target_link_libraries(fastfingers ${CMAKE_THREAD_LIBS_INIT} ${GTK_LIBRARIES}
        ${GM_EXPORT_LIBRARIES})
target_link_libraries(cheatsheet ${CMAKE_THREAD_LIBS_INIT} ${GTK_LIBRARIES}
        ${GM_EXPORT_LIBRARIES})
# Adding necessary compile flags
set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} -Wl,--export-dynamic -rdynamic -Wno-deprecated-declarations -Wall -g"
        )
# Changing cheatsheet's executable name to "fastfingers-cheatsheet"
set_target_properties(cheatsheet PROPERTIES OUTPUT_NAME
        "fastfingers-cheatsheet")

file(
        COPY ${CMAKE_SOURCE_DIR}/../data/applications/
        DESTINATION "$ENV{HOME}/.fastfingers/model"
        FILE_PERMISSIONS
        OWNER_READ OWNER_WRITE
        GROUP_READ GROUP_WRITE
        WORLD_READ WORLD_WRITE
        )

# Creating desktop files
add_desktop(fastfingers ${CMAKE_PROJECT_VERSION})

file(
        COPY ${CMAKE_BINARY_DIR}/org.ccextractor.Cheatsheet.desktop
        DESTINATION "$ENV{HOME}/.fastfingers/model"
        FILE_PERMISSIONS
        OWNER_READ OWNER_WRITE
        GROUP_READ
        WORLD_READ
)

install(
        TARGETS fastfingers cheatsheet
        DESTINATION bin
        PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE)

install(
        FILESCheatsheet
        ${CMAKE_BINARY_DIR}/org.ccextractor.FastFingers.desktop
        ${CMAKE_BINARY_DIR}/org.ccextractor.Cheatsheet.desktop
        DESTINATION
        /usr/share/applications/
        PERMISSIONS
        OWNER_READ OWNER_WRITE
        GROUP_READ
        WORLD_READ
)

# Installing hicolor icons
list(APPEND HICOLOR_PX 16 32 48 64 128)
foreach (PX IN LISTS HICOLOR_PX)
    if (EXISTS "/usr/share/icons/hicolor/${PX}x${PX}/")
        install(
                FILES
                ${CMAKE_SOURCE_DIR}/../data/icons/hicolor/${PX}x${PX}/apps/fastfingers.png
                ${CMAKE_SOURCE_DIR}/../data/icons/hicolor/${PX}x${PX}/apps/fastfingers-cheatsheet.png
                DESTINATION
                /usr/share/icons/hicolor/${PX}x${PX}/apps/
                PERMISSIONS
                OWNER_READ OWNER_WRITE
                GROUP_READ
                WORLD_READ
        )
    endif ()
endforeach ()
# Installing highcontrast icons
list(APPEND HICONTRAST_PX 16 22 24 32 48 256)
foreach (PX IN LISTS HICONTRAST_PX)
    if (EXISTS "/usr/share/icons/HighContrast/${PX}x${PX}/")
        install(
                FILES
                ${CMAKE_SOURCE_DIR}/../data/icons/HighContrast/${PX}x${PX}/apps/fastfingers.png
                ${CMAKE_SOURCE_DIR}/../data/icons/HighContrast/${PX}x${PX}/apps/fastfingers-cheatsheet.png
                DESTINATION
                /usr/share/icons/HighContrast/${PX}x${PX}/apps/
                PERMISSIONS
                OWNER_READ OWNER_WRITE
                GROUP_READ
                WORLD_READ
        )
    endif ()
endforeach ()
# Updating highcontrast cache
install(CODE "execute_process(COMMAND gtk-update-icon-cache -t -f /usr/share/icons/HighContrast/)")
install(CODE "execute_process(COMMAND gtk-update-icon-cache -t -f /usr/share/icons/hicolor/)")

execute_process(
        COMMAND pkg-config glib-2.0 --variable prefix
        OUTPUT_VARIABLE _glib_prefix
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(GSETTINGS_DIR "${_glib_prefix}/share/glib-2.0/schemas" CACHE INTERNAL "")

# Getting the command to compile the schemas
execute_process(
        COMMAND pkg-config gio-2.0 --variable glib_compile_schemas
        OUTPUT_VARIABLE _glib_compile_schemas
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(glib_schema_compiler ${_glib_compile_schemas} CACHE INTERNAL "")

# Moving GSettings file to glib schemas directory
install(
        FILES ${CMAKE_SOURCE_DIR}/../data/org.ccextractor.FastFingers.gschema.xml
        DESTINATION ${GSETTINGS_DIR}
)

# Compiling the GSetting file
install(CODE "execute_process(COMMAND ${glib_schema_compiler} ${GSETTINGS_DIR})")

