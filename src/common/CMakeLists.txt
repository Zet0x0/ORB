add_library(common STATIC)

qt_add_qml_module(
    common
    URI
    orb.common
    OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/orb/common"
    SOURCES
    singleton.h
    utilities.h
    utilities.cpp
    urlvalidator.h
    urlvalidator.cpp)

target_link_libraries(
    common
    PUBLIC Qt6::Qml
    PRIVATE Qt6::Gui)
