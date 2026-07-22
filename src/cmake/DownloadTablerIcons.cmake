set(TABLER_ICONS_VERSION "3.45.0")
set(TABLER_ICONS_SHA256
    "7fc0b57bd994865f2b0aee6a163cdce26da03056ae6d82d38fe3a6b24538cb93")
set(TABLER_ICONS_URL
    "https://github.com/tabler/tabler-icons/archive/refs/tags/v${TABLER_ICONS_VERSION}.tar.gz"
)

set(TABLER_ICONS_CACHE_DIR "${CMAKE_BINARY_DIR}/tabler-icons")
set(TABLER_ICONS_ARCHIVE_PATH
    "${TABLER_ICONS_CACHE_DIR}/tabler-icons-${TABLER_ICONS_VERSION}.tar.gz")
set(TABLER_ICONS_EXTRACT_DIR "${TABLER_ICONS_CACHE_DIR}/extract")

set(TABLER_ICONS_DEST_DIR "${CMAKE_SOURCE_DIR}/external/tabler-icons")

file(MAKE_DIRECTORY "${TABLER_ICONS_CACHE_DIR}")

# Download
if(NOT EXISTS "${TABLER_ICONS_ARCHIVE_PATH}")
    message(STATUS "Downloading tabler-icons ${TABLER_ICONS_VERSION}...")

    set(_archive_tmp "${TABLER_ICONS_ARCHIVE_PATH}.tmp")
    file(
        DOWNLOAD "${TABLER_ICONS_URL}" "${_archive_tmp}"
        SHOW_PROGRESS
        TLS_VERIFY ON
        EXPECTED_HASH SHA256=${TABLER_ICONS_SHA256}
        STATUS _download_status)

    list(GET _download_status 0 _code)

    if(NOT _code EQUAL 0)
        file(REMOVE "${_archive_tmp}")
        message(
            FATAL_ERROR
                "Failed to download tabler-icons ${TABLER_ICONS_VERSION}")
    endif()

    file(RENAME "${_archive_tmp}" "${TABLER_ICONS_ARCHIVE_PATH}")
else()
    message(
        STATUS
            "Using cached tabler-icons archive at ${TABLER_ICONS_ARCHIVE_PATH}")
endif()

# Extract
if(NOT
   EXISTS
   "${TABLER_ICONS_EXTRACT_DIR}/tabler-icons-${TABLER_ICONS_VERSION}/icons/outline"
)
    message(STATUS "Extracting tabler-icons ${TABLER_ICONS_VERSION}...")

    file(ARCHIVE_EXTRACT INPUT "${TABLER_ICONS_ARCHIVE_PATH}" DESTINATION
         "${TABLER_ICONS_EXTRACT_DIR}")
else()
    message(
        STATUS
            "Using already-extracted tabler-icons at ${TABLER_ICONS_EXTRACT_DIR}"
    )
endif()

# Place the outline SVGs where icons.qrc expects them

set(_dest_version_file "${TABLER_ICONS_DEST_DIR}/.version")

set(_dest_version "")
if(EXISTS "${_dest_version_file}")
    file(READ "${_dest_version_file}" _dest_version)
    string(STRIP "${_dest_version}" _dest_version)
endif()

if(NOT _dest_version STREQUAL TABLER_ICONS_VERSION)
    message(
        STATUS
            "Copying tabler-icons outline SVGs to ${TABLER_ICONS_DEST_DIR}...")

    if(EXISTS "${TABLER_ICONS_DEST_DIR}")
        file(REMOVE_RECURSE "${TABLER_ICONS_DEST_DIR}")
    endif()

    set(_extracted_root
        "${TABLER_ICONS_EXTRACT_DIR}/tabler-icons-${TABLER_ICONS_VERSION}")

    file(COPY "${_extracted_root}/icons/outline"
         DESTINATION "${TABLER_ICONS_DEST_DIR}/icons")

    file(WRITE "${_dest_version_file}" "${TABLER_ICONS_VERSION}")
endif()
