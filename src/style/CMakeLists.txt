add_library(style STATIC)

qt_add_qml_module(
    style
    URI
    orb.style
    OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/orb/style"
    QML_FILES
    TextField.qml
    NO_PLUGIN)

target_link_libraries(style PRIVATE qml_components)
