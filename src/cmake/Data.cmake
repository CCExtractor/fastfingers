# Installing JSON files to system
macro(add_data DATA_TARGET)
    file(COPY "${CMAKE_SOURCE_DIR}/../data/applications" DESTINATION "$ENV{HOME}/.fastfingers")
endmacro()
