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
    searcherror.h)

target_link_libraries(sources PRIVATE sourcesplugin)
