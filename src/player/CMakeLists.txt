add_library(player STATIC)

qt_add_qml_module(
    player
    URI
    orb.player
    OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/orb/player"
    DEPENDENCIES
    orb.sources
    SOURCES
    player.h
    player.cpp
    mpvproperties.h)

target_link_libraries(
    player
    PUBLIC MpvQt::MpvQt
    PRIVATE sources common)
