project(NorthEngine)

function(DetectPlatform TARGET_NAME)
    # Detect Platform
    if (WIN32)
        target_compile_definitions(${TARGET_NAME} PRIVATE NE_PLATFORM_WINDOWS)
        target_compile_definitions(${TARGET_NAME} PRIVATE NE_WINDOW_WIN32)
        message(STATUS "Platform: Windows with Win32")

    elseif (APPLE)
        target_compile_definitions(${TARGET_NAME} PRIVATE NE_PLATFORM_APPLE)

        # Check if it's macOS or iOS
        if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            target_compile_definitions(${TARGET_NAME} PRIVATE NE_WINDOW_COCOA)
            message(STATUS "Platform: macOS with Cocoa")
        elseif (${CMAKE_SYSTEM_NAME} MATCHES "iOS")
            target_compile_definitions(${TARGET_NAME} PRIVATE NE_WINDOW_UIKIT)
            message(STATUS "Platform: iOS with UIKit")
        else ()
            target_compile_definitions(${TARGET_NAME} PRIVATE NE_WINDOW_COCOA)
            message(STATUS "Platform: Apple with Cocoa (default)")
        endif ()

    elseif (UNIX AND NOT APPLE)
        target_compile_definitions(${TARGET_NAME} PRIVATE NE_PLATFORM_LINUX)

        # Try to detect Wayland or X11
        # First check if user explicitly specified windowing system
        if (DEFINED NE_FORCE_WAYLAND)
            target_compile_definitions(${TARGET_NAME} PRIVATE NE_WINDOW_WAYLAND)
            message(STATUS "Platform: Linux with Wayland (forced)")

        elseif (DEFINED NE_FORCE_X11)
            target_compile_definitions(${TARGET_NAME} PRIVATE NE_WINDOW_X11)
            message(STATUS "Platform: Linux with X11 (forced)")

        else ()
            # Auto-detect: Check for Wayland first (more modern)
            find_package(PkgConfig QUIET)
            if (PKG_CONFIG_FOUND)
                pkg_check_modules(WAYLAND_CLIENT QUIET wayland-client)
                pkg_check_modules(X11 QUIET x11)
            endif ()

            # Check environment variable as a hint
            if (DEFINED ENV{WAYLAND_DISPLAY})
                set(PREFER_WAYLAND TRUE)
            elseif (DEFINED ENV{DISPLAY})
                set(PREFER_X11 TRUE)
            endif ()

            # Decide based on available libraries and environment
            if (WAYLAND_CLIENT_FOUND AND PREFER_WAYLAND)
                target_compile_definitions(${TARGET_NAME} PRIVATE NE_WINDOW_WAYLAND)
                message(STATUS "Platform: Linux with Wayland (detected)")
            elseif (X11_FOUND OR PREFER_X11)
                target_compile_definitions(${TARGET_NAME} PRIVATE NE_WINDOW_X11)
                message(STATUS "Platform: Linux with X11 (detected)")
            elseif (WAYLAND_CLIENT_FOUND)
                target_compile_definitions(${TARGET_NAME} PRIVATE NE_WINDOW_WAYLAND)
                message(STATUS "Platform: Linux with Wayland (fallback)")
            else ()
                # Default to X11 if nothing is detected
                target_compile_definitions(${TARGET_NAME} PRIVATE NE_WINDOW_X11)
                message(STATUS "Platform: Linux with X11 (default)")
            endif ()
        endif ()

    else ()
        message(WARNING "Unknown platform detected")
        target_compile_definitions(${TARGET_NAME} PRIVATE NE_PLATFORM_UNKNOWN)
        target_compile_definitions(${TARGET_NAME} PRIVATE NE_WINDOW_UNKNOWN)
    endif ()

endfunction()