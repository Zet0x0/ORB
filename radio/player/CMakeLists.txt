add_library(player STATIC)

qt_add_qml_module(
    player
    URI
    radio.player
    SOURCES
    player.h
    player.cpp
    mpvproperties.h
    mpvproperties.cpp)
