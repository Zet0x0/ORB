add_library(logging STATIC)

set_source_files_properties(LogCategories.qml PROPERTIES QT_QML_SINGLETON_TYPE
                                                         TRUE)

qt_add_qml_module(
    logging
    URI
    ORB.Logging
    OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/ORB/Logging"
    SOURCES
    logcategories.h
    logcategories.cpp
    logger.h
    logger.cpp
    logfiltermodel.h
    logfiltermodel.cpp
    QML_FILES
    LogCategories.qml)

target_link_libraries(logging PUBLIC Qt6::Qml)
