#pragma once

#include <sol/sol.hpp>
#include <SDL3/SDL.h>
#include <memory>

#include "../../include/Logger/Logger.hpp"

namespace Tasq
{
 
    class Window
    {
    private:
        std::shared_ptr<SDL_Window> window;
        Logger logger{"Window"};
        sol::state* lua;
    
    public:
        Window();
        ~Window() = default;

        void init(sol::state* luaState) { lua = luaState; };

        void registerFuncs();

        void create(const std::string &title, int width, int height);
    };

}
