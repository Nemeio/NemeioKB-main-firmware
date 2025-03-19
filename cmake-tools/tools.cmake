set(BIN_FILE Scratch-Install-${VERSION}-local-UNSECURE.bin)
configure_file(${CMAKE_CURRENT_LIST_DIR}/flash.in ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink)
configure_file(${CMAKE_CURRENT_LIST_DIR}/erase.in ${CMAKE_CURRENT_BINARY_DIR}/erase.jlink)
