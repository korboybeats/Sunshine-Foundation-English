# windows specific compile definitions

add_compile_definitions(SUNSHINE_PLATFORM="windows")

enable_language(RC)
set(CMAKE_RC_COMPILER windres)
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} --use-temp-file")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

# gcc complains about misleading indentation in some mingw includes
list(APPEND SUNSHINE_COMPILE_OPTIONS -Wno-misleading-indentation)

# see gcc bug 98723
add_definitions(-DUSE_BOOST_REGEX)

# Fix for GCC 15 C++20 coroutine compatibility with WinRT
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 15)
    # Add compiler flags to handle WinRT coroutine issues with GCC 15
    list(APPEND SUNSHINE_COMPILE_OPTIONS -Wno-template-body)
    add_definitions(-DWINRT_LEAN_AND_MEAN)
    # Force using experimental coroutine ABI for compatibility
    add_definitions(-D_ALLOW_COROUTINE_ABI_MISMATCH)
endif()

# curl
add_definitions(-DCURL_STATICLIB)
include_directories(SYSTEM ${CURL_STATIC_INCLUDE_DIRS})
link_directories(${CURL_STATIC_LIBRARY_DIRS})

# miniupnpc
add_definitions(-DMINIUPNP_STATICLIB)

# extra tools/binaries for audio/display devices
add_subdirectory(tools)  # todo - this is temporary, only tools for Windows are needed, for now

# nvidia
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/third-party/nvapi-open-source-sdk")
file(GLOB NVPREFS_FILES CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/third-party/nvapi-open-source-sdk/*.h"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/nvprefs/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/nvprefs/*.h")

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/third-party/nvenc-headers")
file(GLOB_RECURSE NVENC_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/src/nvenc/*.h"
        "${CMAKE_SOURCE_DIR}/src/nvenc/*.cpp")

# vigem
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/third-party/ViGEmClient/include")

# sunshine icon
if(NOT DEFINED SUNSHINE_ICON_PATH)
    set(SUNSHINE_ICON_PATH "${CMAKE_SOURCE_DIR}/sunshine.ico")
endif()

configure_file("${CMAKE_SOURCE_DIR}/src/platform/windows/windows.rc.in" windows.rc @ONLY)

# set(SUNSHINE_TRAY 0)

set(PLATFORM_TARGET_FILES
        "${CMAKE_CURRENT_BINARY_DIR}/windows.rc"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/publish.cpp"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/misc.h"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/misc.cpp"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/input.cpp"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/display.h"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/display_base.cpp"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/display_vram.cpp"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/display_ram.cpp"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/display_wgc.cpp"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/display_amd.cpp"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/audio.cpp"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/mic_write.cpp"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/display_device/device_hdr_states.cpp"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/display_device/device_modes.cpp"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/display_device/device_topology.cpp"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/display_device/general_functions.cpp"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/display_device/settings_topology.cpp"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/display_device/settings_topology.h"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/display_device/settings.cpp"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/display_device/windows_utils.h"
        "${CMAKE_SOURCE_DIR}/src/platform/windows/display_device/windows_utils.cpp"
        "${CMAKE_SOURCE_DIR}/third-party/ViGEmClient/src/ViGEmClient.cpp"
        "${CMAKE_SOURCE_DIR}/third-party/ViGEmClient/include/ViGEm/Client.h"
        "${CMAKE_SOURCE_DIR}/third-party/ViGEmClient/include/ViGEm/Common.h"
        "${CMAKE_SOURCE_DIR}/third-party/ViGEmClient/include/ViGEm/Util.h"
        "${CMAKE_SOURCE_DIR}/third-party/ViGEmClient/include/ViGEm/km/BusShared.h"
        ${NVPREFS_FILES}
        ${NVENC_SOURCES})

set(OPENSSL_LIBRARIES
        libssl.a
        libcrypto.a)

list(PREPEND PLATFORM_LIBRARIES
        ${CURL_STATIC_LIBRARIES}
        avrt
        d3d11
        D3DCompiler
        dwmapi
        dxgi
        iphlpapi
        ksuser
        libssp.a
        libstdc++.a
        libwinpthread.a
        minhook::minhook
        nlohmann_json::nlohmann_json
        ntdll
        ole32
        setupapi
        shlwapi
        synchronization.lib
        userenv
        ws2_32
        wsock32
)

if(SUNSHINE_ENABLE_TRAY)
    list(APPEND PLATFORM_TARGET_FILES
            "${CMAKE_SOURCE_DIR}/third-party/tray/src/tray_windows.c")
endif()