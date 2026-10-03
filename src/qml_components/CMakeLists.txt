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
    FavoriteButton.qml
    VolumeControl.qml
    ElapsedLabel.qml
    SearchTextField.qml
    StationNameLabel.qml
    NowPlayingLabel.qml
    MarqueeLabel.qml
    StationSearch.qml
    LayoutScrollBar.qml
    AboutDialog.qml
    OpenLocationDialog.qml
    SystemTrayIcon.qml
    SystemTrayMenu.qml
    PreferencesDialog.qml
    LogDialog.qml
    ErrorBanner.qml
    PlaybackStatusIndicator.qml
    TickedSlider.qml
    NO_PLUGIN)

target_link_libraries(qml_components PRIVATE common logging player settings
                                             sources style)
