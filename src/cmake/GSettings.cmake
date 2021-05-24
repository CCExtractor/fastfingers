# Getting the glib directory location
execute_process(
  COMMAND pkg-config glib-2.0 --variable prefix
  OUTPUT_VARIABLE _glib_prefix
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )
set(GSETTINGS_DIR "${_glib_prefix}/share/glib-2.0/schemas" CACHE INTERNAL "")
# Moving GSettings file to glib schemas directory
file(COPY ${CMAKE_SOURCE_DIR}/../data/org.ccextractor.FastFingers.gschema.xml
  DESTINATION ${GSETTINGS_DIR})
# Getting the schema compiler path
execute_process(
  COMMAND pkg-config gio-2.0 --variable glib_compile_schemas
  OUTPUT_VARIABLE _glib_compile_schemas
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )
set(glib_schema_compiler ${_glib_compile_schemas} CACHE INTERNAL "")
# Installing the GSetting file
install(
  FILES ${GSETTINGS_FILE}
  DESTINATION ${GSETTINGS_DIR}
  )
# Compiling the GSetting file
execute_process(
  COMMAND ${glib_schema_compiler} ${GSETTINGS_DIR}
  OUTPUT_VARIABLE _glib_prefix
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )
