set(MPV_CACHE_DIR "${CMAKE_BINARY_DIR}/mpv")
set(MPV_DOWNLOAD_DIR "${MPV_CACHE_DIR}/download")
set(MPV_EXTRACT_DIR "${MPV_CACHE_DIR}/libmpv")

file(MAKE_DIRECTORY "${MPV_DOWNLOAD_DIR}")
file(MAKE_DIRECTORY "${MPV_EXTRACT_DIR}")

set(MPV_GITHUB_API
    "https://api.github.com/repos/shinchiro/mpv-winbuild-cmake/releases/latest")

if(NOT EXISTS "${MPV_DOWNLOAD_DIR}/latest_release.json")
    message(STATUS "Fetching latest mpv release info...")

    file(
        DOWNLOAD "${MPV_GITHUB_API}" "${MPV_DOWNLOAD_DIR}/latest_release.json"
        STATUS _status
        SHOW_PROGRESS)

    list(GET _status 0 _code)

    if(NOT _code EQUAL 0)
        message(FATAL_ERROR "Failed to fetch latest mpv release info.")
    endif()
endif()

file(READ "${MPV_DOWNLOAD_DIR}/latest_release.json" MPV_RELEASE_JSON)

# Extract the first asset URL that matches mpv-dev-x86_64-v3-*.7z
string(
    REGEX MATCHALL
          "\"browser_download_url\": \"([^\"]*mpv-dev-x86_64-v3-[^\"]*\\.7z)\""
          _matches "${MPV_RELEASE_JSON}")

if(NOT _matches)
    message(
        FATAL_ERROR
            "Could not find mpv-dev-x86_64-v3-*.7z asset in latest release JSON"
    )
endif()

# Use the first match
string(REGEX REPLACE "\"browser_download_url\": \"([^\"]*)\"" "\\1" MPV_URL
                     "${_matches}")

get_filename_component(MPV_FILENAME "${MPV_URL}" NAME)
set(MPV_ARCHIVE_PATH "${MPV_DOWNLOAD_DIR}/${MPV_FILENAME}")

# Download
if(NOT EXISTS "${MPV_ARCHIVE_PATH}")
    message(STATUS "Downloading ${MPV_FILENAME}...")

    file(
        DOWNLOAD "${MPV_URL}" "${MPV_ARCHIVE_PATH}"
        SHOW_PROGRESS
        STATUS _download_status)

    list(GET _download_status 0 _code)

    if(NOT _code EQUAL 0)
        message(FATAL_ERROR "Failed to download ${MPV_FILENAME}")
    endif()
endif()

# Extract
if(NOT EXISTS "${MPV_EXTRACT_DIR}/include")
    message(STATUS "Extracting ${MPV_FILENAME}...")

    file(ARCHIVE_EXTRACT INPUT "${MPV_ARCHIVE_PATH}" DESTINATION
         "${MPV_EXTRACT_DIR}")
endif()

# Set CMake variables
set(Libmpv_INCLUDE_DIRS
    "${MPV_EXTRACT_DIR}/include"
    CACHE PATH "Include directories for libmpv")
set(Libmpv_LIBRARIES
    "${MPV_EXTRACT_DIR}/libmpv.dll.a"
    CACHE FILEPATH "Path to libmpv import library")
