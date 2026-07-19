set(SOURCES main.cpp singleton.h utilities.h utilities.cpp)
qt_add_big_resources(SOURCES icons.qrc)
list(APPEND SOURCES icons.qrc)

qt_add_executable(radio ${SOURCES})

qt_add_qml_module(radio URI radio QML_FILES Main.qml)

add_subdirectory(player)
add_subdirectory(sources)
add_subdirectory(qml_components)

target_link_libraries(radio PRIVATE Qt6::Quick Qt6::QuickControls2 player
                                    sources qml_components)
