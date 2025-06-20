cmake_minimum_required(VERSION 3.15)
project(MonnyEngine VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

# Diretórios
set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external")
set(INSTALL_DIR "${CMAKE_SOURCE_DIR}/dist")
set(CMAKE_INSTALL_PREFIX "${INSTALL_DIR}" CACHE PATH "Install path prefix" FORCE)

# ⚠️ Forçar tudo como biblioteca STATIC para que o CMake compile os objetos diretamente no .exe
set(BUILD_SHARED_LIBS OFF)
set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_SHARED OFF CACHE BOOL "" FORCE)
set(SDL3TTF_SHARED OFF CACHE BOOL "" FORCE)

# Adiciona bibliotecas externas como código (serão embutidas)
add_subdirectory(${EXTERNAL_DIR}/SDL2)
add_subdirectory(${EXTERNAL_DIR}/SDL2_image)
add_subdirectory(${EXTERNAL_DIR}/SDL2_ttf)
add_subdirectory(${EXTERNAL_DIR}/lua)
add_subdirectory(${EXTERNAL_DIR}/fmt)
add_subdirectory(${EXTERNAL_DIR}/spdlog)
add_subdirectory(${EXTERNAL_DIR}/sol2)

# Executável principal
add_executable(tEngine
    src/Launcher/Launcher.cpp
    src/Engine/Engine.cpp
    src/Logger/Logger.cpp
)

target_include_directories(tEngine PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# ⚠️ Linka com libs em modo estático (mas geradas como objetos e embutidas no .exe)
target_link_libraries(tEngine PRIVATE
    SDL3-static
    SDL3_image-static
    SDL3_ttf-static
    fmt::fmt
    spdlog::spdlog
    sol2
    lua
)

# Instalar o executável (somente ele — nada de .so)
install(TARGETS tEngine RUNTIME DESTINATION bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)
