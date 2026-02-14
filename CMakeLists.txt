cmake_minimum_required(VERSION 4.0)
project(MonnyEngine VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    if (MSVC)
        add_compile_options(/W4)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic)
    endif()
        
    add_compile_definitions(D_WIN32)
    set(PLATFORM win32)

elseif(UNIX AND NOT WIN32)
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_definitions(D_LINUX)
    set(PLATFORM linux)
else()
    message(FATAL "sistema operacional desconhecido. Apenas aceitos \"Windows\" e \"Linux\"")
endif()

include_directories(include)

file(GLOB_RECURSE SOURCES "src/${PLATFORM}/*.c")

add_executable(mengine ${SOURCES})