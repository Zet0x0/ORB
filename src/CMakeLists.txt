cmake_minimum_required(VERSION 3.20)

project(
    ORB
    VERSION 0.1.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

set(BUILD_SHARED_LIBS ON)

include("cmake/DownloadLibmpv.cmake")
include("cmake/DownloadTablerIcons.cmake")

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2 LabsPlatform)

qt_standard_project_setup(REQUIRES 6.11)

add_subdirectory("external/mpvqt" EXCLUDE_FROM_ALL)

set(SOURCES ORB.rc main.cpp)
qt_add_big_resources(SOURCES icons/icons.qrc)
qt_add_executable(ORB ${SOURCES})

qt_add_qml_module(ORB URI orb QML_FILES Main.qml)

add_subdirectory(common)
add_subdirectory(settings)
add_subdirectory(sources)
add_subdirectory(player)
add_subdirectory(qml_components)

target_link_libraries(
    ORB
    PRIVATE Qt6::Quick
            Qt6::QuickControls2
            Qt6::LabsPlatform
            settings
            sources
            player
            qml_components)

set_target_properties(
    ORB PROPERTIES WIN32_EXECUTABLE TRUE RUNTIME_OUTPUT_DIRECTORY
                                         "${CMAKE_CURRENT_BINARY_DIR}/bin")

set(CMAKE_INSTALL_BINDIR ".")
include(GNUInstallDirs)
install(TARGETS ORB RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES "${Libmpv_RUNTIME_DLL}" DESTINATION "${CMAKE_INSTALL_BINDIR}")
if(BUILD_SHARED_LIBS)
    install(FILES "$<TARGET_FILE:MpvQt>" DESTINATION "${CMAKE_INSTALL_BINDIR}")
endif()

qt_generate_deploy_app_script(
    TARGET
    ORB
    OUTPUT_SCRIPT
    deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
    DEPLOY_TOOL_OPTIONS
    --qmldir
    "${CMAKE_CURRENT_SOURCE_DIR}")

install(SCRIPT "cmake/DeploySettings.cmake")
install(SCRIPT "${deploy_script}")
