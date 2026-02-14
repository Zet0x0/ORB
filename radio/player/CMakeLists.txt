add_library(player STATIC)

qt_add_qml_module(
    player
    URI
    radio.player
    SOURCES
    player.h
    player.cpp
    mpvproperties.h
    station.h
    station.cpp
    stationmodel.h
    stationmodel.cpp)

target_link_libraries(
    player
    PUBLIC MpvQt::MpvQt
    PRIVATE playerplugin)
