cmake_minimum_required(VERSION 3.20)

project(
    ORB
    VERSION 0.1.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

include("cmake/DownloadLibmpv.cmake")

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)

add_subdirectory("external/mpvqt")

qt_standard_project_setup(REQUIRES 6.10)

set(SOURCES windows/ORB.rc main.cpp singleton.h utilities.h utilities.cpp)
qt_add_big_resources(SOURCES icons.qrc)
list(APPEND SOURCES icons.qrc)
qt_add_executable(ORB ${SOURCES})
qt_add_qml_module(ORB URI orb QML_FILES Main.qml)

add_subdirectory(player)
add_subdirectory(sources)
add_subdirectory(qml_components)

target_link_libraries(ORB PRIVATE Qt6::Quick Qt6::QuickControls2 player sources
                                  qml_components)

set_target_properties(
    ORB PROPERTIES WIN32_EXECUTABLE TRUE RUNTIME_OUTPUT_DIRECTORY
                                         "${CMAKE_CURRENT_BINARY_DIR}/bin")

include(GNUInstallDirs)
install(
    TARGETS ORB
    BUNDLE DESTINATION "."
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
