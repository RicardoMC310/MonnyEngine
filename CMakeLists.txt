cmake_minimum_required(VERSION 4.0)
project(MonnyEngine VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SDL_SHARED ON CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)

set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_WERROR OFF CACHE BOOL "" FORCE)

set(CMAKE_BUILD_TYPE Debug)

if (WIN32)
    set(PLATFORM win32)
elseif(UNIX AND NOT WIN32)
    set(PLATFORM linux)
else()
    message(FATAL "sistema operacional desconhecido. Apenas aceitos \"Windows\" e \"Linux\"")
endif()

file(GLOB_RECURSE SOURCES "src/${PLATFORM}/*.c")

list(APPEND SOURCES "vendor/cJSON/cJSON.c")

add_subdirectory(external/lua-wrapper)
add_subdirectory(vendor/SDL)
add_subdirectory(vendor/SDL_image)

add_executable(mengine ${SOURCES})

target_link_libraries(mengine PRIVATE lua)
target_link_libraries(mengine PRIVATE SDL3::SDL3)
target_link_libraries(mengine PRIVATE m)

target_include_directories(mengine PRIVATE include)
target_include_directories(mengine PRIVATE vendor/cJSON)
target_include_directories(mengine PRIVATE vendor/SDL/include)

target_compile_definitions(mengine PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)

if (WIN32)

    if (MSVC)
        target_compile_options(mengine PRIVATE \W4)
    else()
        target_compile_options(mengine PRIVATE -Wall -Wextra -Wpedantic)
    endif()
        
    target_compile_definitions(mengine PRIVATE D_WIN32)
    set(PLATFORM win32)

elseif(UNIX AND NOT WIN32)
    target_compile_options(mengine PRIVATE  -Wall -Wextra -Wpedantic)
    target_compile_definitions(mengine PRIVATE  D_LINUX)
    set(PLATFORM linux)
else()
    message(FATAL "sistema operacional desconhecido. Apenas aceitos \"Windows\" e \"Linux\"")
endif()