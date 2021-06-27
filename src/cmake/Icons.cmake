# Installing app icons
macro(add_icons ICONS_TARGET)
  # Installing hicolor icons
  list(APPEND HICOLOR_PX 16 32 48 64 128)
  foreach (PX IN LISTS HICOLOR_PX)
    if (EXISTS "/usr/share/icons/hicolor/${PX}x${PX}/")
      add_custom_command (
	TARGET ${ICONS_TARGET}
	POST_BUILD
	COMMAND
	${CMAKE_COMMAND} -E copy
	"${CMAKE_SOURCE_DIR}/../data/icons/hicolor/${PX}x${PX}/fastfingers.png"
	"/usr/share/icons/hicolor/${PX}x${PX}/apps/"
	)
    endif()
  endforeach()
  # Updating hicolor cache
  add_custom_command(
    TARGET fastfingers
    POST_BUILD
    COMMAND gtk-update-icon-cache -t -f /usr/share/icons/hicolor/
    )
  # Installing highcontrast icons
  list(APPEND HICONTRAST_PX 16 22 24 32 48 256)
  foreach (PX IN LISTS HICONTRAST_PX)
    if (EXISTS "/usr/share/icons/HighContrast/${PX}x${PX}/")
      add_custom_command (
	TARGET fastfingers
	POST_BUILD
	COMMAND
	${CMAKE_COMMAND} -E copy
	"${CMAKE_SOURCE_DIR}/../data/icons/HighContrast/${PX}x${PX}/fastfingers.png"
	"/usr/share/icons/HighContrast/${PX}x${PX}/apps/"
	)
    endif()
  endforeach()
  # Updating highcontrast cache
  add_custom_command(
    TARGET fastfingers
    POST_BUILD
    COMMAND gtk-update-icon-cache -t -f /usr/share/icons/HighContrast/
    )
endmacro(add_icons)
