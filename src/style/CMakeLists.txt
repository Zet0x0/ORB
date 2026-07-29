add_library(style STATIC)

qt_add_qml_module(
    style
    URI
    ORB.Style
    OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/ORB/Style"
    QML_FILES
    AppPalette.qml
    TextField.qml
    BusyIndicator.qml
    NO_PLUGIN)

target_link_libraries(style PRIVATE qml_components)
