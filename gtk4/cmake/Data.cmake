# Installing JSON files to system
macro(add_data DATA_TARGET)
  add_custom_command(
    TARGET ${DATA_TARGET}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/../data/applications"
    "/usr/share/fastfingers/applications"
    )
endmacro()
