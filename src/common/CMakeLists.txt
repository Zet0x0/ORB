add_library(common STATIC)

qt_add_qml_module(
    common
    URI
    ORB.Common
    OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/ORB/Common"
    SOURCES
    singleton.h
    utilities.h
    utilities.cpp
    urlvalidator.h
    urlvalidator.cpp
    errorinfo.h
    errorinfo.cpp)

target_link_libraries(
    common
    PUBLIC Qt6::Qml
    PRIVATE Qt6::Gui)
