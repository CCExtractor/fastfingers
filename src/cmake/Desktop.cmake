# Installing .desktop file to the system
macro(add_desktop DESKTOP_TARGET VERSION)
  set(_d_version ${VERSION})
  configure_file(
    "${CMAKE_SOURCE_DIR}/../data/org.ccextractor.FastFingers.desktop.in"
    "${CMAKE_BINARY_DIR}/org.ccextractor.FastFingers.desktop" @ONLY)
  configure_file(
    "${CMAKE_SOURCE_DIR}/../data/org.ccextractor.Cheatsheet.desktop.in"
    "${CMAKE_BINARY_DIR}/org.ccextractor.Cheatsheet.desktop" @ONLY)
endmacro()
