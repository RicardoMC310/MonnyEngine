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

    class RequestCreateWindowEvent final : public Event {
    public:
        std::string title;
        uint width = 0, height = 0;

        std::string getType() const override { return "RequestCreateWindowEvent"; }
    };

    class OnEventUserInterface final : public Event {
    public:
        SDL_Event event = {};

        std::string getType() const override { return "OnKeyEvent"; }
    };

    class SystemExitEvent final : public Event {
    public:
        int exitCode = 0;

        std::string getType() const override { return "SystemExitEvent"; }
    };
}

#endif //EVENTS_H
