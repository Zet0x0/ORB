add_library(player STATIC)

qt_add_qml_module(
    player
    URI
    orb.player
    OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/orb/player"
    SOURCES
    player.h
    player.cpp
    mpvproperties.h
    station.h
    station.cpp)

target_link_libraries(
    player
    PUBLIC MpvQt::MpvQt
    PRIVATE playerplugin)
