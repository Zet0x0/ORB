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
    stationmodel.h
    stationmodel.cpp
    station.h
    station.cpp
    providers/nullsource.h
    providers/nullsource.cpp
    providers/radiorecord.h
    providers/radiorecord.cpp)

target_link_libraries(sources PRIVATE common)
