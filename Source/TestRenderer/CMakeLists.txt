cmake_minimum_required(VERSION 2.8.12)

project(TestRenderer)

set(COMMON_SOURCE_FILES
    Precompiled.h
    Precompiled.cpp
    Application.h
    Application.cpp)

set(WINDOWS_SOURCE_FILES
    WinMain.cpp
    WinResource.h
    WinRes/small.ico
    WinRes/TestRenderer.ico
    WinRes/TestRenderer.rc)

set(MACOS_SOURCE_FILES
    macOSMain.mm
    macOSMainMenu.xib)

set(IOS_SOURCE_FILES
    iOSMain.mm
    iOSLaunchScreen.xib)

set(ANDROID_SOURCE_FILES
    android_native_app_glue.h
    android_native_app_glue.c
    AndroidMain.cpp)

if (NOT PLATFORM_WINDOWS)
    list(APPEND COMPILING_DISABLED_SOURCES ${WINDOWS_SOURCE_FILES})
endif ()

if (NOT PLATFORM_MACOS)
    list(APPEND COMPILING_DISABLED_SOURCES ${MACOS_SOURCE_FILES})
endif ()

if (NOT PLATFORM_IOS)
    list(APPEND COMPILING_DISABLED_SOURCES ${IOS_SOURCE_FILES})
endif ()

if (NOT PLATFORM_ANDROID)
    list(APPEND COMPILING_DISABLED_SOURCES ${ANDROID_SOURCE_FILES})
endif ()

set_source_files_properties(${COMPILING_DISABLED_SOURCES} PROPERTIES HEADER_FILE_ONLY TRUE)

set(ALL_FILES
    ${COMMON_SOURCE_FILES}
    ${WINDOWS_SOURCE_FILES}
    ${MACOS_SOURCE_FILES}
    ${IOS_SOURCE_FILES}
    ${ANDROID_SOURCE_FILES})

auto_source_group(${ALL_FILES})

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${ENGINE_INCLUDE_DIR}/Runtime/Public
    ${ENGINE_INCLUDE_DIR}/ThirdParty)

enable_precompiled_header(Precompiled.h Precompiled.cpp ALL_FILES)

if (ANDROID)
    # Export ANativeActivity_onCreate(),
    # Refer to: https://github.com/android-ndk/ndk/issues/381.
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

    add_library(${PROJECT_NAME} SHARED ${ALL_FILES})
elseif (APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${ALL_FILES})
elseif (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${ALL_FILES})
endif ()

if (NOT ANDROID AND NOT IOS)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        PREFIX ""
        #DEVELOPMENT_POSTFIX "_dev"
        OUTPUT_NAME ${PROJECT_NAME}
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Bin/${ENGINE_BUILD_PLATFORM_DIR})
endif ()

if (ANDROID)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "\"${CMAKE_SOURCE_DIR}/Bin/Android/Assets\""
        COMMAND ${CMAKE_COMMAND} -E copy_directory "\"${CMAKE_SOURCE_DIR}/Data\"" "\"${CMAKE_SOURCE_DIR}/Bin/Android/Assets/Data\""
        COMMENT Copying asset files)
elseif (APPLE)
    # App name
    set(CMAKEVAR_APP_NAME "Test Renderer")
    # App version
    set(CMAKEVAR_BUNDLE_SHORT_VERSION_STRING "1.0")
    # Bundle version
    set(CMAKEVAR_BUNDLE_VERSION "1")

    set_target_properties(${PROJECT_NAME} PROPERTIES 
        XCODE_ATTRIBUTE_PRODUCT_NAME "TestRenderer"
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.polygontek.TestRenderer"
        XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "gnu++14"
        XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++"
        XCODE_ATTRIBUTE_CLANG_MODULES_AUTOLINK "NO"
        XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC "YES"
        XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS "NO"
        XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Debug] "YES"
        XCODE_ATTRIBUTE_GCC_C_LANGUAGE_STANDARD "gnu11"
        XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
        XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Debug] "dwarf"
        XCODE_ATTRIBUTE_COPY_PHASE_STRIP "YES"
        XCODE_ATTRIBUTE_COPY_PHASE_STRIP[variant=Debug] "NO"
        #XCODE_ATTRIBUTE_GCC_PREFIX_HEADER ${CMAKE_CURRENT_LIST_DIR}/Precompiled.h
        #XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES"
        XCODE_ATTRIBUTE_INSTALL_PATH "/Applications"
        XCODE_ATTRIBUTE_SKIP_INSTALL "NO"
        XCODE_ATTRIBUTE_MTL_FAST_MATH "YES"
        XCODE_ATTRIBUTE_MTL_ENABLE_DEBUG_INFO "NO"
        XCODE_ATTRIBUTE_MTL_ENABLE_DEBUG_INFO[variant=Debug] "INCLUDE_SOURCE")

    if (IOS)
        set_target_properties(${PROJECT_NAME} PROPERTIES 
            XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET ${IOS_DEPLOYMENT_TARGET}
            XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2" # Universal (iPad + iPhone)
            XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
            MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/iOSplist.in)

        add_framework(${PROJECT_NAME} Foundation)
        add_framework(${PROJECT_NAME} UIKit)
    else ()
        set(CMAKEVAR_COPYRIGHT "Copyright (c) 2014 PolygonTek. All rights reserved.")

        set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/macOSplist.in)

        set_source_files_properties(macOSMainMenu.xib PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

        add_framework(${PROJECT_NAME} Foundation)
        add_framework(${PROJECT_NAME} AppKit)
    endif ()

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND rm -rf "\"$(TARGET_BUILD_DIR)/$(PRODUCT_NAME).app/Data\""
        COMMAND cp -Rf "\"$(PROJECT_DIR)/Data\"" "\"$(TARGET_BUILD_DIR)/$(PRODUCT_NAME).app/Data\""
        COMMENT Copying data files)
elseif (WIN32)
    target_link_libraries(${PROJECT_NAME} winmm.lib)

    if (MSVC)
        set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE YES)
    endif ()
endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Test)

target_link_libraries(${PROJECT_NAME} BlueshiftRuntime)
