add_library(sources STATIC)

qt_add_qml_module(
    sources
    URI
    orb.sources
    OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/orb/sources"
    SOURCES
    sourcecontroller.h
    sourcecontroller.cpp
    source.h
    source.cpp
    sourceerror.h
    sourceerror.cpp
    nullsource.h
    nullsource.cpp
    stationmodel.h
    stationmodel.cpp
    radiorecord.h
    radiorecord.cpp
    station.h
    station.cpp)
