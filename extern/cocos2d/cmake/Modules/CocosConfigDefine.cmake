 #Please use them everywhere
 #WINDOWS   =   Windows Desktop
 #ANDROID    =  Android
 #IOS    =  iOS
 #MACOSX    =  MacOS X
 #LINUX      =   Linux
 if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
     set(WINDOWS TRUE)
     if(CMAKE_CL_64) #x64
        set(X64 TRUE)
        set(PLATFORM_FOLDER win64)
        message("x64")
     else()
        message("x86")
        set(PLATFORM_FOLDER win32)
     endif()
 elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
     set(PLATFORM_FOLDER android)
 elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
     if(ANDROID)
         set(PLATFORM_FOLDER android)
     else()
         set(LINUX TRUE)
         set(PLATFORM_FOLDER linux)
     endif()
 elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
     if(IOS)
         set(APPLE TRUE)
         set(PLATFORM_FOLDER ios)
     else()
         set(APPLE TRUE)
         set(MACOSX TRUE)
         set(PLATFORM_FOLDER mac)
     endif()
else()
     message(FATAL_ERROR "Unsupported platform, CMake will exit")
     return()
 endif()

 # build mode, Debug is default value
 if(NOT CMAKE_BUILD_TYPE)
     if(DEBUG_MODE)
         set(CMAKE_BUILD_TYPE Debug)
     else()
         set(CMAKE_BUILD_TYPE Release)
     endif()
 endif()

 if(CMAKE_GENERATOR)
    # generators that are capable of organizing into a hierarchy of folders
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    # simplify generator condition judgement
    if(CMAKE_GENERATOR STREQUAL Xcode)
        set(XCODE TRUE)
    elseif(CMAKE_GENERATOR MATCHES Visual)
        set(VS TRUE)
    endif()
    # make configurations type keep same to cmake build type.
    set(CMAKE_CONFIGURATION_TYPES "${CMAKE_BUILD_TYPE}" CACHE STRING "Reset the configurations to what we need" FORCE)
 endif()
 message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")


# custom target property for dll collect
define_property(TARGET
    PROPERTY CC_DEPEND_DLLS
    BRIEF_DOCS "depend dlls of a target"
    FULL_DOCS "use to save depend dlls of a target"
)
# custom target property for lua/js link
define_property(TARGET
    PROPERTY CC_JS_DEPEND
    BRIEF_DOCS "cocos2d js depend libs"
    FULL_DOCS "use to save depend libs of cocos2d js project"
) 
define_property(TARGET
    PROPERTY CC_LUA_DEPEND
    BRIEF_DOCS "cocos2d lua depend libs"
    FULL_DOCS "use to save depend libs of cocos2d lua project"
) 

# check c++ standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# check visual studio version
 if(WINDOWS)
    # not support other compile tools except MSVC for now
    if(MSVC)
        # Visual Studio 2015, MSVC_VERSION 1900      (v140 toolset)
        # Visual Studio 2017, MSVC_VERSION 1910-1919 (v141 toolset)
        if(${MSVC_VERSION} EQUAL 1900 OR ${MSVC_VERSION} GREATER 1900)
            message(STATUS "using Windows MSVC generate cocos2d-x project, MSVC_VERSION:${MSVC_VERSION}")
        else()
            message(FATAL_ERROR "using Windows MSVC generate cocos2d-x project, MSVC_VERSION:${MSVC_VERSION} lower than needed")
        endif()
    else()
        message(FATAL_ERROR "please using Windows MSVC compile cocos2d-x project, support other compile tools not yet")
    endif()
endif()

 # Set macro definitions for special platforms
 function(use_cocos2dx_compile_define target)
    target_compile_definitions(${target} PUBLIC "COCOS2D_DEBUG$<$<CONFIG:Debug>:=1>")
    if(APPLE)
        target_compile_definitions(${target} PUBLIC __APPLE__)
        target_compile_definitions(${target} PUBLIC USE_FILE32API)
    elseif(LINUX)
        target_compile_definitions(${target} PUBLIC LINUX)
        target_compile_definitions(${target} PUBLIC _GNU_SOURCE)
    elseif(ANDROID)
        target_compile_definitions(${target} PUBLIC ANDROID)
        target_compile_definitions(${target} PUBLIC USE_FILE32API)
    elseif(WINDOWS)
        if(X64)
        target_compile_definitions(${target} 
            PUBLIC WIN64
            PUBLIC _WIN64
            PUBLIC _WINDOWS
            PUBLIC UNICODE
            PUBLIC _UNICODE
            PUBLIC _CRT_SECURE_NO_WARNINGS
            PUBLIC _SCL_SECURE_NO_WARNINGS
        )
        else()
            target_compile_definitions(${target} 
            PUBLIC WIN32
            PUBLIC _WIN32
            PUBLIC _WINDOWS
            PUBLIC UNICODE
            PUBLIC _UNICODE
            PUBLIC _CRT_SECURE_NO_WARNINGS
            PUBLIC _SCL_SECURE_NO_WARNINGS
        )
        endif()
        if(BUILD_SHARED_LIBS)
            target_compile_definitions(${target} 
                PUBLIC _USRDLL
                PUBLIC _EXPORT_DLL_
                PUBLIC _USEGUIDLL
                PUBLIC _USREXDLL
                PUBLIC _USRSTUDIODLL
                PUBLIC _USE3DDLL
            )
        else()
            target_compile_definitions(${target} PUBLIC CC_STATIC)
        endif()
    endif()
endfunction()

 # Set compiler options
 function(use_cocos2dx_compile_options target)
    if(MSVC)
        target_compile_options(${target}
            PUBLIC /MP
        )
    endif()
 endfunction()
