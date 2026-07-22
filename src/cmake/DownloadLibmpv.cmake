set(MPV_CACHE_DIR "${CMAKE_BINARY_DIR}/mpv")
set(MPV_DOWNLOAD_DIR "${MPV_CACHE_DIR}/download")
set(MPV_EXTRACT_DIR "${MPV_CACHE_DIR}/libmpv")

file(MAKE_DIRECTORY "${MPV_DOWNLOAD_DIR}")
file(MAKE_DIRECTORY "${MPV_EXTRACT_DIR}")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

set(MPV_GITHUB_API
    "https://api.github.com/repos/shinchiro/mpv-winbuild-cmake/releases/latest")

# Cached for the lifetime of the build directory; delete it to pick a newer
# release
set(MPV_RELEASE_JSON_PATH "${MPV_DOWNLOAD_DIR}/latest_release.json")
if(NOT EXISTS "${MPV_RELEASE_JSON_PATH}")
    message(STATUS "Fetching latest mpv release info...")

    set(_json_tmp "${MPV_RELEASE_JSON_PATH}.tmp")
    file(
        DOWNLOAD "${MPV_GITHUB_API}" "${_json_tmp}"
        STATUS _status
        TLS_VERIFY ON
        SHOW_PROGRESS)

    list(GET _status 0 _code)

    if(NOT _code EQUAL 0)
        file(REMOVE "${_json_tmp}")
        message(FATAL_ERROR "Failed to fetch latest mpv release info")
    endif()

    file(RENAME "${_json_tmp}" "${MPV_RELEASE_JSON_PATH}")
else()
    message(
        STATUS
            "Using cached mpv release info at ${MPV_RELEASE_JSON_PATH} (delete it to re-check upstream)"
    )
endif()

file(READ "${MPV_RELEASE_JSON_PATH}" MPV_RELEASE_JSON)

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
list(GET _matches 0 _first_match)
string(REGEX REPLACE "\"browser_download_url\": \"([^\"]*)\"" "\\1" MPV_URL
                     "${_first_match}")

get_filename_component(MPV_FILENAME "${MPV_URL}" NAME)
set(MPV_ARCHIVE_PATH "${MPV_DOWNLOAD_DIR}/${MPV_FILENAME}")

message(STATUS "Resolved mpv download URL: ${MPV_URL}")

# Download
if(NOT EXISTS "${MPV_ARCHIVE_PATH}")
    message(STATUS "Downloading ${MPV_FILENAME}...")

    set(_archive_tmp "${MPV_ARCHIVE_PATH}.tmp")
    file(
        DOWNLOAD "${MPV_URL}" "${_archive_tmp}"
        SHOW_PROGRESS
        TLS_VERIFY ON
        STATUS _download_status)

    list(GET _download_status 0 _code)

    if(NOT _code EQUAL 0)
        file(REMOVE "${_archive_tmp}")
        message(FATAL_ERROR "Failed to download ${MPV_FILENAME}")
    endif()

    file(RENAME "${_archive_tmp}" "${MPV_ARCHIVE_PATH}")
else()
    message(STATUS "Using cached archive at ${MPV_ARCHIVE_PATH}")
endif()

# Extract
if(NOT EXISTS "${MPV_EXTRACT_DIR}/include")
    message(STATUS "Extracting ${MPV_FILENAME}...")

    file(ARCHIVE_EXTRACT INPUT "${MPV_ARCHIVE_PATH}" DESTINATION
         "${MPV_EXTRACT_DIR}")
else()
    message(STATUS "Using already-extracted mpv at ${MPV_EXTRACT_DIR}")
endif()

# Copy libmpv-2.dll to build_dir/bin
file(COPY_FILE "${MPV_EXTRACT_DIR}/libmpv-2.dll"
     "${CMAKE_CURRENT_BINARY_DIR}/bin/libmpv-2.dll" ONLY_IF_DIFFERENT)

# Set CMake variables
set(Libmpv_INCLUDE_DIRS
    "${MPV_EXTRACT_DIR}/include"
    CACHE PATH "Include directories for libmpv")
set(Libmpv_LIBRARIES
    "${MPV_EXTRACT_DIR}/libmpv.dll.a"
    CACHE FILEPATH "Path to libmpv import library")
set(Libmpv_RUNTIME_DLL
    "${MPV_EXTRACT_DIR}/libmpv-2.dll"
    CACHE FILEPATH "Path to the libmpv runtime DLL")
