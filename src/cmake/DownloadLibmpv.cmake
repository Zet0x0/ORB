set(MPV_VERSION "20260901")
set(MPV_GIT_HASH "02a595ddc1")
set(MPV_ARCHIVE_SHA256
    "680feac97f2da3721e331d6b10d2d0e3e02f1113be068fac06aff7f833a165d4")

set(MPV_ARCHIVE_NAME "mpv-dev-x86_64-${MPV_VERSION}-git-${MPV_GIT_HASH}.7z")
set(MPV_URL
    "https://github.com/shinchiro/mpv-winbuild-cmake/releases/download/${MPV_VERSION}/${MPV_ARCHIVE_NAME}"
)

set(MPV_CACHE_DIR "${CMAKE_BINARY_DIR}/mpv")
set(MPV_ARCHIVE_PATH "${MPV_CACHE_DIR}/${MPV_ARCHIVE_NAME}")
set(MPV_EXTRACT_DIR "${MPV_CACHE_DIR}/libmpv")

file(MAKE_DIRECTORY "${MPV_CACHE_DIR}")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

# Download
if(NOT EXISTS "${MPV_ARCHIVE_PATH}")
    message(STATUS "Downloading ${MPV_ARCHIVE_NAME}...")

    set(_archive_tmp "${MPV_ARCHIVE_PATH}.tmp")
    file(
        DOWNLOAD "${MPV_URL}" "${_archive_tmp}"
        SHOW_PROGRESS
        TLS_VERIFY ON
        EXPECTED_HASH SHA256=${MPV_ARCHIVE_SHA256}
        STATUS _download_status)

    list(GET _download_status 0 _code)

    if(NOT _code EQUAL 0)
        file(REMOVE "${_archive_tmp}")
        message(FATAL_ERROR "Failed to download ${MPV_ARCHIVE_NAME}")
    endif()

    file(RENAME "${_archive_tmp}" "${MPV_ARCHIVE_PATH}")
else()
    message(STATUS "Using cached archive at ${MPV_ARCHIVE_PATH}")
endif()

# Extract (re-extract when the pinned archive changes)
set(_extract_stamp "${MPV_EXTRACT_DIR}/.archive")

set(_extracted "")
if(EXISTS "${_extract_stamp}")
    file(READ "${_extract_stamp}" _extracted)
    string(STRIP "${_extracted}" _extracted)
endif()

if(NOT _extracted STREQUAL MPV_ARCHIVE_NAME)
    message(STATUS "Extracting ${MPV_ARCHIVE_NAME}...")

    file(REMOVE_RECURSE "${MPV_EXTRACT_DIR}")
    file(ARCHIVE_EXTRACT INPUT "${MPV_ARCHIVE_PATH}" DESTINATION
         "${MPV_EXTRACT_DIR}")

    file(WRITE "${_extract_stamp}" "${MPV_ARCHIVE_NAME}")
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
