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

add_subdirectory(radio)

set_target_properties(
    radio PROPERTIES WIN32_EXECUTABLE TRUE RUNTIME_OUTPUT_DIRECTORY
                                           "${CMAKE_CURRENT_BINARY_DIR}/bin")

include(GNUInstallDirs)
install(
    TARGETS radio
    BUNDLE DESTINATION "."
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
