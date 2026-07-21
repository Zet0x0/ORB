add_library(qml_components STATIC)

qt_add_qml_module(
    qml_components
    URI
    radio.qml_components
    QML_FILES
    StationImage.qml
    StationDelegate.qml
    IconButton.qml
    PlayButton.qml
    ElapsedLabel.qml
    StationSearchField.qml
    StationNameLabel.qml
    NowPlayingLabel.qml
    StationSearch.qml)

target_link_libraries(qml_components PRIVATE qml_componentsplugin)
