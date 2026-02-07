cmake_minimum_required(VERSION 3.16)

project(
    Unnamed-Radio-App
    VERSION 0.1.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/DownloadLibmpv.cmake)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)

add_subdirectory(external/mpvqt)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(radio radio/main.cpp)

qt_add_qml_module(radio URI radio QML_FILES radio/Main.qml)

set_target_properties(radio PROPERTIES WIN32_EXECUTABLE TRUE)

target_link_libraries(radio PRIVATE Qt6::Quick Qt6::QuickControls2 MpvQt::MpvQt)

include(GNUInstallDirs)
install(
    TARGETS radio
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
