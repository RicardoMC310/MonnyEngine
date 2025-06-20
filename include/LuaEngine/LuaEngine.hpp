#pragma once

#include <sol/sol.hpp>

#include "../../include/Logger/Logger.hpp"

namespace Tasq
{

    class LuaEngine
    {
    private:
        sol::state lua;
        Logger logger{"LuaEngine"};
        Logger lua_logger{"Lua"};

    public:
        LuaEngine();
        ~LuaEngine() = default;

        void loadScript(const std::string& script_path);

        void registerLoggers();

        sol::state& getLuaState() { return lua; }

    };

}