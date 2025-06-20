#pragma once

#include <string>

#include "../Logger/Logger.hpp"
#include "../LuaEngine/LuaEngine.hpp"
#include "../Window/Window.hpp"

namespace Tasq
{
    class Engine
    {
    private:
        Logger logger{"Engine"};
        LuaEngine luaEngine;
        Window window;

        sol::function startFunction;
        sol::function updateFunction;
        sol::function cleanup;

        bool isRunning = false;

    public:
        Engine();
        ~Engine() = default;

        void registerFuncs();

        void runArchive(const std::string &archivePath);

        void run();

    };
}