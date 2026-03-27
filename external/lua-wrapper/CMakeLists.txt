file(GLOB LUA_SOURCES
    ${CMAKE_SOURCE_DIR}/vendor/lua/*.c
)

list(REMOVE_ITEM LUA_SOURCES
    ${CMAKE_SOURCE_DIR}/vendor/lua/lua.c
    ${CMAKE_SOURCE_DIR}/vendor/lua/luac.c
    ${CMAKE_SOURCE_DIR}/vendor/lua/onelua.c
)

add_library(lua STATIC ${LUA_SOURCES})

target_include_directories(lua PUBLIC
    ${CMAKE_SOURCE_DIR}/vendor/lua
)