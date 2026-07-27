add_library(sources STATIC)

qt_add_qml_module(
    sources
    URI
    ORB.Sources
    OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/ORB/Sources"
    DEPENDENCIES
    ORB.Common
    SOURCES
    sourcecontroller.h
    sourcecontroller.cpp
    source.h
    source.cpp
    stationmodel.h
    stationmodel.cpp
    station.h
    station.cpp
    providers/nullsource.h
    providers/nullsource.cpp
    providers/radiorecord.h
    providers/radiorecord.cpp)

target_link_libraries(sources PRIVATE common)
