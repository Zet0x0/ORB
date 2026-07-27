add_library(qml_components STATIC)

qt_add_qml_module(
    qml_components
    URI
    ORB.QmlComponents
    OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/ORB/QmlComponents"
    QML_FILES
    MainWindow.qml
    StationImage.qml
    StationDelegate.qml
    IconButton.qml
    PlayButton.qml
    VolumeControl.qml
    ElapsedLabel.qml
    StationSearchField.qml
    StationNameLabel.qml
    NowPlayingLabel.qml
    StationSearch.qml
    AboutDialog.qml
    OpenLocationDialog.qml
    SystemTrayIcon.qml
    SystemTrayMenu.qml
    TextFieldContextMenu.qml
    PreferencesDialog.qml
    NO_PLUGIN)
