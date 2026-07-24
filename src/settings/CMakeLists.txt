add_library(settings STATIC)

qt_add_qml_module(
    settings
    URI
    orb.settings
    OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/orb/settings"
    DEPENDENCIES
    orb.sources
    SOURCES
    settings.h
    settings.cpp
    settingsfactory.h
    settingsfactory.cpp
    settingsio.h
    settingsio.cpp
    windowsettings.h
    windowsettings.cpp
    sourcessettings.h
    sourcessettings.cpp
    playersettings.h
    playersettings.cpp
    traysettings.h
    traysettings.cpp)

target_link_libraries(settings PRIVATE sources common)
