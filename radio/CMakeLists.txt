qt_add_executable(radio main.cpp singleton.h utilities.h utilities.cpp)

qt_add_qml_module(radio URI radio QML_FILES Main.qml)

add_subdirectory(player)
add_subdirectory(sources)

target_link_libraries(radio PRIVATE Qt6::Quick Qt6::QuickControls2 player
                                    sources)
