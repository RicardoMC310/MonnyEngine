//
// Created by ricardo on 18/07/25.
//

#ifndef EVENTS_H
#define EVENTS_H
#include <string>
#include <sol/variadic_args.hpp>
#include <SDL2/SDL.h>

namespace Monny {

    class Event {
    public:
        virtual ~Event() = default;

        virtual std::string getType() const { return "Event"; }
    };

    class MomentCreateAFunctionInLuaEvent final : public Event {
    public:
        sol::state* lua;

        std::string getType() const override { return "MomentCreateAFunctionInLuaEvent"; }
    };

    class OnKeyEvent final : public Event {
    public:
        std::string type, key;

        std::string getType() const override { return "OnKeyEvent"; }
    };

    class OnMouseButtonEvent final : public Event {
    public:
        std::string button, state;
        int x, y, clicks;

        std::string getType() const override { return "OnMouseButtonEvent"; }
    };

    class OnMouseMotionEvent final : public Event {
    public:
        std::string state;
        int x, y, xrel ,yrel;

        std::string getType() const override { return "OnMouseMotionEvent"; }
    };

    class OnMouseWheelEvent final : public Event {
    public:
        int x, y, preciseX ,preciseY;

        std::string getType() const override { return "OnMouseMotionEvent"; }
    };

    class WindowCreatedEvent final : public Event {
    public:
        SDL_Window* window;

        std::string getType() const override { return "WindowCreatedEvent"; }
    };

    class SystemExitEvent final : public Event {
    public:
        int exitCode = 0;

        std::string getType() const override { return "SystemExitEvent"; }
    };
}

#endif //EVENTS_H
