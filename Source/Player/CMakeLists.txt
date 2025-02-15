cmake_minimum_required(VERSION 2.8.12)

# Variables are given by Blueshift Editor
set(BS_APP_NAME "Blueshift Player" CACHE STRING "BS_APP_NAME variable defaulting")
set(BS_APP_IDENTIFIER "com.polygontek.BlueshiftPlayer" CACHE STRING "BS_APP_IDENTIFIER variable defaulting")
set(BS_APP_VERSION "1.0" CACHE STRING "BS_APP_VERSION variable defaulting")
set(BS_PRODUCT_NAME "BlueshiftPlayer" CACHE STRING "BS_PRODUCT_NAME variable defaulting")
set(BS_TARGET_DEVICE "1,2" CACHE STRING "BS_TARGET_DEVICE variable defaulting")
set(BS_ORIENTATION "<string>UIInterfaceOrientationPortrait</string>" CACHE STRING "BS_ORIENTATION variable defaulting")
set(BS_USE_ANALYTICS FALSE CACHE BOOL "BS_USE_ANALYTICS variable defaulting")
set(BS_USE_ADMOB FALSE CACHE BOOL "BS_USE_ADMOB variable defaulting")

project(BlueshiftPlayer)

set(COMMON_SOURCE_FILES
    Precompiled.h
    Precompiled.cpp
    Application.h
    Application.cpp)

set(WINDOWS_SOURCE_FILES
    WinMain.cpp
    WinResource.h
    Manifest.xml
    WinRes/player.rc
    WinRes/player.ico)

set(MACOS_SOURCE_FILES
    macOSMain.mm
    macOSMainMenu.xib)

set(ANDROID_SOURCE_FILES
    android_native_app_glue.h
    android_native_app_glue.c
    AndroidMain.cpp
    AndroidAnalytics.h
    AndroidAnalytics.cpp
    AndroidAdMob.h
    AndroidAdMob.cpp)

set(IOS_SOURCE_FILES
    iOSMain.mm
    iOSLaunchScreen.xib
    Assets.xcassets
    iOSAnalytics.h
    iOSAnalytics.mm
    iOSAdMob.h
    iOSAdMob.mm)

if (NOT PLATFORM_WINDOWS)
    list(APPEND COMPILING_DISABLED_SOURCES ${WINDOWS_SOURCE_FILES})
endif ()

if (NOT PLATFORM_MACOS)
    list(APPEND COMPILING_DISABLED_SOURCES ${MACOS_SOURCE_FILES})
endif ()

if (NOT PLATFORM_ANDROID)
    list(APPEND COMPILING_DISABLED_SOURCES ${ANDROID_SOURCE_FILES})
endif ()

if (NOT PLATFORM_IOS)
    list(APPEND COMPILING_DISABLED_SOURCES ${IOS_SOURCE_FILES})
endif ()

set_source_files_properties(${COMPILING_DISABLED_SOURCES} PROPERTIES HEADER_FILE_ONLY TRUE)

if (PLATFORM_WINDOWS)
    set_source_files_properties(${WINDOWS_SOURCE_FILES} PROPERTIES HEADER_FILE_ONLY FALSE)
elseif (PLATFORM_MACOS)
    set_source_files_properties(${MACOS_SOURCE_FILES} PROPERTIES HEADER_FILE_ONLY FALSE)
elseif (PLATFORM_ANDROID)
    set_source_files_properties(${ANDROID_SOURCE_FILES} PROPERTIES HEADER_FILE_ONLY FALSE)

    if (NOT BS_USE_ANALYTICS)
        set_source_files_properties(AndroidAnalytics.h AndroidAnalytics.cpp PROPERTIES HEADER_FILE_ONLY TRUE)
    endif ()

    if (NOT BS_USE_ADMOB)
        set_source_files_properties(AndroidAdMob.h AndroidAdMob.cpp PROPERTIES HEADER_FILE_ONLY TRUE)
    endif ()
elseif (PLATFORM_IOS)
    set_source_files_properties(${IOS_SOURCE_FILES} PROPERTIES HEADER_FILE_ONLY FALSE)

    if (NOT BS_USE_ANALYTICS)
        set_source_files_properties(iOSAnalytics.h iOSAnalytics.mm PROPERTIES HEADER_FILE_ONLY TRUE)
    endif ()

    if (NOT BS_USE_ADMOB)
        set_source_files_properties(iOSAdMob.h iOSAdMob.mm PROPERTIES HEADER_FILE_ONLY TRUE)
    endif ()
endif ()

set(ALL_FILES
    ${COMMON_SOURCE_FILES}
    ${WINDOWS_SOURCE_FILES}
    ${MACOS_SOURCE_FILES}
    ${ANDROID_SOURCE_FILES}
    ${IOS_SOURCE_FILES})

auto_source_group(${ALL_FILES})

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${ENGINE_INCLUDE_DIR}/Runtime/Public
    ${ENGINE_INCLUDE_DIR}/ThirdParty)

enable_precompiled_header(Precompiled.h Precompiled.cpp ALL_FILES)

if (PLATFORM_ANDROID)
    # Export ANativeActivity_onCreate(),
    # Refer to: https://github.com/android-ndk/ndk/issues/381.
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

    add_library(${PROJECT_NAME} SHARED ${ALL_FILES})
elseif (APPLE)
    if (USE_LUAJIT AND NOT IOS)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pagezero_size 10000 -image_base 100000000")
    endif ()

    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${ALL_FILES})
elseif (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${ALL_FILES})
endif ()

if (PLATFORM_ANDROID OR PLATFORM_IOS)
    if (BS_USE_ANALYTICS)
        add_definitions(-DUSE_ANALYTICS=1)
    endif ()

    if (BS_USE_ADMOB)
        add_definitions(-DUSE_ADMOB=1)
    endif ()
else ()
    set_target_properties(${PROJECT_NAME} PROPERTIES
        PREFIX ""
        #DEVELOPMENT_POSTFIX "_dev"
        OUTPUT_NAME ${PROJECT_NAME}
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Bin/${ENGINE_BUILD_PLATFORM_DIR})
endif ()

if (APPLE)
    # App name
    set(CMAKEVAR_APP_NAME ${BS_APP_NAME})
    # App version
    set(CMAKEVAR_BUNDLE_SHORT_VERSION_STRING ${BS_APP_VERSION})
    # Bundle version
    set(CMAKEVAR_BUNDLE_VERSION "1")
    # Orientation
    set(CMAKEVAR_ORIENTATION ${BS_ORIENTATION})

    set_target_properties(${PROJECT_NAME} PROPERTIES 
        XCODE_ATTRIBUTE_PRODUCT_NAME ${BS_PRODUCT_NAME}
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER ${BS_APP_IDENTIFIER}
        XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "gnu++14"
        XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++"
        XCODE_ATTRIBUTE_CLANG_MODULES_AUTOLINK "NO"
        XCODE_ATTRIBUTE_CLANG_ENABLE_MODULES "YES"
        XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC "YES"
        XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_WEAK "YES"
        XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS "NO"
        XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Debug] "YES"
        XCODE_ATTRIBUTE_GCC_C_LANGUAGE_STANDARD "gnu11"
        XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
        XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Debug] "dwarf"
        XCODE_ATTRIBUTE_COPY_PHASE_STRIP "YES"
        XCODE_ATTRIBUTE_COPY_PHASE_STRIP[variant=Debug] "NO"
        XCODE_ATTRIBUTE_GCC_PREFIX_HEADER "${PROJECT_SOURCE_DIR}/Precompiled.h"
        XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES"
        XCODE_ATTRIBUTE_INSTALL_PATH "/Applications"
        XCODE_ATTRIBUTE_SKIP_INSTALL "NO"
        XCODE_ATTRIBUTE_MTL_FAST_MATH "YES"
        XCODE_ATTRIBUTE_MTL_ENABLE_DEBUG_INFO "NO"
        XCODE_ATTRIBUTE_MTL_ENABLE_DEBUG_INFO[variant=Debug] "INCLUDE_SOURCE")

    if (PLATFORM_IOS)
        set_target_properties(${PROJECT_NAME} PROPERTIES 
            XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET ${IOS_DEPLOYMENT_TARGET}
            XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY ${BS_TARGET_DEVICE}
            XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
            #XCODE_ATTRIBUTE_DEVELOPMENT_TEAM XXX
            XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
            MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/iOSplist.in)

        set_source_files_properties(iOSLaunchScreen.xib Assets.xcassets PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

        add_framework(${PROJECT_NAME} Foundation)
        add_framework(${PROJECT_NAME} UIKit)
        add_framework(${PROJECT_NAME} OpenGLES)
        add_framework(${PROJECT_NAME} CoreGraphics)
        add_framework(${PROJECT_NAME} QuartzCore)
        add_framework(${PROJECT_NAME} OpenAL)
        add_framework(${PROJECT_NAME} AudioToolbox)
        add_framework(${PROJECT_NAME} AVFoundation)

        if (BS_USE_ANALYTICS)
            set(GoogleAnalyticsServices ${ENGINE_INCLUDE_DIR}/ThirdParty/GoogleAnalyticsServices)

            if (${GoogleAnalyticsServices} STREQUAL GoogleAnalyticsServices-NOTFOUND)
                message(ERROR ": GoogleAnalyticsServices folder not found")
            else ()
                target_link_libraries(${PROJECT_NAME} ${GoogleAnalyticsServices}/libGoogleAnalyticsServices.a)

                target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${GoogleAnalyticsServices}/GoogleAnalytics/Library>)
            endif ()

            find_package(ZLIB REQUIRED)
            target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})

            find_library(SQLITE3_LIBRARY NAMES sqlite3)
            target_link_libraries(${PROJECT_NAME} ${SQLITE3_LIBRARY})

            add_framework(${PROJECT_NAME} CoreData)
            add_framework(${PROJECT_NAME} SystemConfiguration)
        endif ()
            
        if (BS_USE_ADMOB)
            set(GoogleMobileAds ${ENGINE_INCLUDE_DIR}/ThirdParty/GoogleMobileAds/GoogleMobileAds)

            if (${GoogleMobileAds} STREQUAL GoogleMobileAds-NOTFOUND)
                message(ERROR ": Framework GoogleMobileAds not found")
            else ()
                target_link_libraries(${PROJECT_NAME} ${GoogleMobileAds}.framework)

                target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${GoogleMobileAds}.framework/Headers>)
            endif ()

            add_framework(${PROJECT_NAME} AdSupport)
            add_framework(${PROJECT_NAME} CFNetwork)
            add_framework(${PROJECT_NAME} CoreMedia)
            add_framework(${PROJECT_NAME} CoreMotion)
            add_framework(${PROJECT_NAME} CoreTelephony)
            add_framework(${PROJECT_NAME} CoreVideo)
            add_framework(${PROJECT_NAME} GLKit)
            add_framework(${PROJECT_NAME} MessageUI)
            add_framework(${PROJECT_NAME} MediaPlayer)
            add_framework(${PROJECT_NAME} MobileCoreServices)
            add_framework(${PROJECT_NAME} Security)
            add_framework(${PROJECT_NAME} StoreKit)
            add_framework(${PROJECT_NAME} SystemConfiguration)
        endif ()
    elseif (PLATFORM_MACOS)
        set(CMAKEVAR_COPYRIGHT "Copyright (c) 2014 PolygonTek. All rights reserved.")

        set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/macOSplist.in)

        set_source_files_properties(macOSMainMenu.xib PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

        add_framework(${PROJECT_NAME} Foundation)
        add_framework(${PROJECT_NAME} AppKit)
        add_framework(${PROJECT_NAME} Carbon)
        add_framework(${PROJECT_NAME} OpenGL)
        add_framework(${PROJECT_NAME} CoreGraphics)
        add_framework(${PROJECT_NAME} CoreVideo)
        add_framework(${PROJECT_NAME} OpenAL)
        add_framework(${PROJECT_NAME} AudioToolbox)
        add_framework(${PROJECT_NAME} AVFoundation)
    endif ()
elseif (PLATFORM_WINDOWS)
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} winmm.lib imm32.lib dxguid.lib dsound.lib ws2_32.lib)

    if (MSVC)
        set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE YES)

        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "mt.exe" -nologo -manifest "${PROJECT_SOURCE_DIR}\\Manifest.xml" -outputresource:"$<TARGET_FILE:${PROJECT_NAME}>\;1"
            COMMENT "Adding manifest...")
    endif ()
endif ()

if (BUILD_RUNTIME)
    set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Blueshift)

    target_link_libraries(${PROJECT_NAME} BlueshiftRuntime)
elseif (BUILD_PLAYER)
    set(THIRDPARTY_LIB_FILES
        zlib
        minizip
        libjpeg
        libpng
        libpvrt
        etcpack_lib
        Etc2CompLib
        libogg
        libvorbis
        freetype
        jsoncpp
        GGL
        imgui
        Bullet
        HACD
        STB
        LuaCpp
        luasocket)

    if (USE_LUAJIT)
        include(FindLuaJIT)
        target_link_libraries(${PROJECT_NAME} ${LUAJIT_LIBRARY})

        include_directories(${PROJECT_NAME} ${ENGINE_INCLUDE_DIR}/ThirdParty/luaJIT/src)
    else ()
        list(APPEND THIRDPARTY_LIB_FILES lua)

        include_directories(${PROJECT_NAME} ${ENGINE_INCLUDE_DIR}/ThirdParty/lua)
    endif ()

    set(IMPORTED_LIB_FILES ${THIRDPARTY_LIB_FILES} BlueshiftRuntime)

    if (PLATFORM_ANDROID)
        target_link_libraries(${PROJECT_NAME} 
            android
            atomic
            log
            EGL
            GLESv3
            OpenSLES)

        foreach (FILE ${IMPORTED_LIB_FILES})
            add_library(${FILE} STATIC IMPORTED)

            set_target_properties(${FILE} PROPERTIES
                IMPORTED_LOCATION ${ENGINE_LIBRARY_DIR}/${ENGINE_BUILD_PLATFORM_DIR}/${FILE}${CMAKE_STATIC_LIBRARY_SUFFIX})
        endforeach ()
    else ()
        foreach (FILE ${IMPORTED_LIB_FILES})
            add_library(${FILE} STATIC IMPORTED)

            message(STATUS ${ENGINE_LIBRARY_DIR}/${ENGINE_BUILD_PLATFORM_DIR}/Debug/${FILE}${CMAKE_STATIC_LIBRARY_SUFFIX})

            set_target_properties(${FILE} PROPERTIES
                IMPORTED_LOCATION_DEBUG   ${ENGINE_LIBRARY_DIR}/${ENGINE_BUILD_PLATFORM_DIR}/Debug/${FILE}${CMAKE_STATIC_LIBRARY_SUFFIX}
                IMPORTED_LOCATION_RELEASE ${ENGINE_LIBRARY_DIR}/${ENGINE_BUILD_PLATFORM_DIR}/Release/${FILE}${CMAKE_STATIC_LIBRARY_SUFFIX})
        endforeach ()
    endif ()
 
    if (APPLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            #COMMAND rm -rf "\"$(TARGET_BUILD_DIR)/$(PRODUCT_NAME).app/Data\""
            COMMAND mkdir -p "\"$(PROJECT_DIR)/Scripts\""
            COMMAND cp -Rf "\"$(PROJECT_DIR)/Data\"" "\"$(TARGET_BUILD_DIR)/$(PRODUCT_NAME).app\""
            COMMAND cp -Rf "\"$(PROJECT_DIR)/Config\"" "\"$(TARGET_BUILD_DIR)/$(PRODUCT_NAME).app\""
            COMMENT "Copying data files"
            USES_TERMINAL)
    endif ()

    set_property(TARGET minizip PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES zlib)
    set_property(TARGET libpng PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES zlib)
    set_property(TARGET libvorbis PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES libogg)
    set_property(TARGET BlueshiftRuntime PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES ${THIRDPARTY_LIB_FILES})

    target_link_libraries(${PROJECT_NAME} ${THIRDPARTY_LIB_FILES} BlueshiftRuntime)
endif ()
