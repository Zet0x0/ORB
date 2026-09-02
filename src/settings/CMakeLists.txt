add_library(settings STATIC)

qt_add_qml_module(
    settings
    URI
    ORB.Settings
    OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/ORB/Settings"
    DEPENDENCIES
    ORB.Sources
    SOURCES
    settings.h
    settings.cpp
    settingsfactory.h
    settingsfactory.cpp
    settingsio.h
    settingsio.cpp
    settingsfieldmeta.h
    settingsgroup.h
    settingsgroup.cpp
    settingsintrospection.h
    settingsintrospection.cpp
    settingscategorymodel.h
    settingscategorymodel.cpp
    settingspropertymodel.h
    settingspropertymodel.cpp
    windowsettings.h
    windowsettings.cpp
    sourcessettings.h
    sourcessettings.cpp
    playersettings.h
    playersettings.cpp
    traysettings.h
    traysettings.cpp
    interfacesettings.h
    interfacesettings.cpp)

target_link_libraries(settings PRIVATE sources common)
