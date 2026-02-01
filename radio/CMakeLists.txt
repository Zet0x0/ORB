cmake_minimum_required(VERSION 3.16)

project(
    Unnamed-Radio-App
    VERSION 0.1
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(radio main.cpp)

qt_add_qml_module(radio URI radio QML_FILES Main.qml)

set_target_properties(
    radio
    PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER "com.zet0x0.radio"
               MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
               MACOSX_BUNDLE_SHORT_VERSION_STRING
               ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
               MACOSX_BUNDLE TRUE
               WIN32_EXECUTABLE TRUE)

target_link_libraries(radio PRIVATE Qt6::Quick Qt6::QuickControls2)

include(GNUInstallDirs)
install(
    TARGETS radio
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
