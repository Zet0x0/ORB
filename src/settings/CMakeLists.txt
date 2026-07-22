add_library(settings STATIC)

qt_add_qml_module(
    settings
    URI
    orb.settings
    OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/orb/settings"
    SOURCES
    settings.h
    settings.cpp
    windowsettings.h
    windowsettings.cpp
    settingsfactory.h
    settingsfactory.cpp
    settingsio.h
    settingsio.cpp)

target_link_libraries(settings PRIVATE common)
