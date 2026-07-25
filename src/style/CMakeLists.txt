add_library(style STATIC)

qt_add_qml_module(
    style
    URI
    ORB.Style
    OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/ORB/Style"
    QML_FILES
    TextField.qml
    BusyIndicator.qml
    NO_PLUGIN)

set(STYLE_RESOURCE_SOURCES)
qt_add_big_resources(STYLE_RESOURCE_SOURCES icons/icons.qrc)
target_sources(ORB PRIVATE ${STYLE_RESOURCE_SOURCES})

target_link_libraries(style PRIVATE qml_components)
