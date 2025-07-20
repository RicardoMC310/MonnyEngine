cmake_minimum_required(VERSION 3.15)
project(MonnyEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)

# Define os diretórios de include
include_directories(include)

# Adiciona o executável
add_executable(MonnyEngine
        src/Launcher.cpp
        src/Core/CoreSystem.cpp
        include/Core/CoreSystem.h
        src/Event/EventSystem.cpp
        include/Event/EventSystem.h
        include/Core/ISystem.h
        include/Event/Events.h
        src/Lua/LuaSystem.cpp
        include/Lua/LuaSystem.h
        src/Window/WindowSystem.cpp
        include/Window/WindowSystem.h
        src/Renderer/RenderSystem.cpp
        include/Renderer/RenderSystem.h
)

# Localize e linke bibliotecas
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

# Biblioteca Lua e sol2 (assumindo vendorizado ou instalado via pacote)
find_package(Lua REQUIRED)

find_path(SOL2_INCLUDE_DIR sol/sol.hpp
        PATHS /usr/include /usr/local/include
)

if(NOT SOL2_INCLUDE_DIR)
    message(FATAL_ERROR "sol2 não encontrado. Defina SOL2_INCLUDE_DIR.")
endif()

include_directories(${SOL2_INCLUDE_DIR})

# Linkagem
target_link_libraries(MonnyEngine
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
        SDL2_ttf::SDL2_ttf
        spdlog::spdlog
        fmt::fmt
        lua
)

# Instalação
install(TARGETS MonnyEngine DESTINATION ${CMAKE_SOURCE_DIR}/dist/bin)
install(DIRECTORY include/ DESTINATION ${CMAKE_SOURCE_DIR}/dist/include)
