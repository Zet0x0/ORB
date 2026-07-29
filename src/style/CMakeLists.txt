add_library(style STATIC)

set_source_files_properties(AppColors.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(
    style
    URI
    ORB.Style
    OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/ORB/Style"
    QML_FILES
    AppColors.qml
    AppPalette.qml
    TextField.qml
    TextFieldContextMenu.qml
    BusyIndicator.qml
    NO_PLUGIN)
