add_library(player STATIC)

qt_add_qml_module(
    player
    URI
    ORB.Player
    OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/ORB/Player"
    DEPENDENCIES
    ORB.Common
    ORB.Sources
    SOURCES
    player.h
    player.cpp
    mpvproperties.h)

target_link_libraries(
    player
    PUBLIC MpvQt::MpvQt
    PRIVATE sources common)
