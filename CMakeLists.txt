cmake_minimum_required(VERSION 3.10)

# Define caminho base para libs externas
set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external")

project(MonnyEngine VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist)

# ---------------------------------------------------
# Bibliotecas externas

# sol2
add_subdirectory(${EXTERNAL_DIR}/sol2)

# fmt (usa target moderno fmt::fmt)
add_subdirectory(${EXTERNAL_DIR}/fmt)

# spdlog (usa target spdlog::spdlog)
add_subdirectory(${EXTERNAL_DIR}/spdlog)

# Lua: biblioteca estática IMPORTADA, já compilada e presente em external/lua
set(LUA_INCLUDE_DIR "${EXTERNAL_DIR}/lua")
set(LUA_LIBRARY "${EXTERNAL_DIR}/lua/liblua.a")

add_library(lua STATIC IMPORTED)
set_target_properties(lua PROPERTIES
    IMPORTED_LOCATION ${LUA_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES ${LUA_INCLUDE_DIR}
)

# ---------------------------------------------------
# Executável do projeto

add_executable(tLauncher
    src/Launcher/Launcher.cpp
    src/Logger/Logger.cpp
    src/Scripting/LuaEngine.cpp
    src/Window/Window.cpp
)

# Includes do projeto + SFML (caminho manual)
target_include_directories(tLauncher PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${EXTERNAL_DIR}/SFML-install/include
)

# Linkagem das libs SFML estáticas manualmente, + libs externas com targets modernos
target_link_libraries(tLauncher PRIVATE
    # SFML estático (confirme nomes dos arquivos na sua pasta lib)
    ${EXTERNAL_DIR}/SFML-install/lib/libsfml-graphics-s.a
    ${EXTERNAL_DIR}/SFML-install/lib/libsfml-window-s.a
    ${EXTERNAL_DIR}/SFML-install/lib/libsfml-system-s.a

    sol2
    lua

    fmt::fmt
    spdlog::spdlog
)

# ---------------------------------------------------
# Instalação do executável e headers do projeto

install(TARGETS tLauncher
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)

# ---------------------------------------------------
# Gerar pacote (CPack)
include(CPack)
