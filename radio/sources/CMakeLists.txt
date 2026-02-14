add_library(sources STATIC)

qt_add_qml_module(
    sources
    URI
    radio.sources
    SOURCES
    sourcecontroller.h
    sourcecontroller.cpp
    source.h
    source.cpp
    searcherror.h
    searchresultmodel.h
    searchresultmodel.cpp
    nullsource.h
    nullsource.cpp)

target_link_libraries(sources PRIVATE sourcesplugin)
