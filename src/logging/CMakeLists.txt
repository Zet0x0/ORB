add_library(logging STATIC)

qt_add_qml_module(
    logging
    URI
    ORB.Logging
    OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/ORB/Logging"
    SOURCES
    logger.h
    logger.cpp
    logfiltermodel.h
    logfiltermodel.cpp)

target_link_libraries(
    logging
    PUBLIC Qt6::Qml
    PRIVATE common)
