//
// Created by ricardo on 19/07/25.
//
#include "../../include/Window/WindowSystem.h"

#include <sol/state.hpp>

#include "Event/Events.h"
#include "Event/EventSystem.h"

namespace Monny {
    void WindowSystem::start() {
        logger.info("Iniciando...");

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            logger.critical("Erro ao iniciar o SDL2");
        }

        this->eventSystem->listener<MomentCreateAFunctionInLuaEvent>([&](const MomentCreateAFunctionInLuaEvent& event) {
                event.lua->set_function("createWindow", [&](std::string title, uint width, uint height) {
                    window = SDL_CreateWindow(
                    title.c_str(),
                    SDL_WINDOWPOS_CENTERED,
                    SDL_WINDOWPOS_CENTERED,
                    width, height,
                    SDL_WINDOW_SHOWN
                );

                if (!window) {
                    logger.critical("Erro ao criar janela: {}", SDL_GetError());
                }

                renderSystem.name = "RenderSystem";
                renderSystem.eventSystem = this->eventSystem;
                renderSystem.logger = Logger("RenderSystem");
                renderSystem.windowRef = window;

                renderSystem.start();

            });
        });

    }

    std::string WindowSystem::getMouseButtonName(Uint8 button) {
        switch (button) {
            case SDL_BUTTON_LEFT: return "Button Left";
            case SDL_BUTTON_RIGHT: return "Button Right";
            case SDL_BUTTON_MIDDLE: return "Button Middle";
            case SDL_BUTTON_X1: return "Button Extra 1";
            case SDL_BUTTON_X2: return "Button Extra 2";
            default: return "Unknown";
        }
    }

    void WindowSystem::run() {
        if (window) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT: {
                        SystemExitEvent eventExit;
                        this->eventSystem->dispatch<SystemExitEvent>(eventExit);
                        break;
                    }

                    case SDL_KEYDOWN:
                    case SDL_KEYUP: {
                        OnKeyEvent eventKey;

                        eventKey.type = event.type == SDL_KEYDOWN ? "Pressed" : "Released";
                        eventKey.key = SDL_GetKeyName(event.key.keysym.sym);

                        this->eventSystem->dispatch<OnKeyEvent>(eventKey);

                        break;
                    }

                    case SDL_MOUSEBUTTONDOWN:
                    case SDL_MOUSEBUTTONUP: {
                        OnMouseButtonEvent eventMouse;

                        eventMouse.button = getMouseButtonName(event.button.button);
                        eventMouse.state = event.button.state == SDL_PRESSED ? "Pressed" : "Released";
                        eventMouse.clicks = event.button.clicks;
                        eventMouse.x = event.button.x;
                        eventMouse.y = event.button.y;

                        this->eventSystem->dispatch<OnMouseButtonEvent>(eventMouse);

                        break;
                    }

                    case SDL_MOUSEMOTION: {
                        OnMouseMotionEvent eventMouse;

                        eventMouse.state = event.motion.state == SDL_PRESSED ? "Pressed" : "Released";
                        eventMouse.x = event.motion.x;
                        eventMouse.y = event.motion.y;
                        eventMouse.xrel = event.motion.xrel;
                        eventMouse.yrel = event.motion.yrel;

                        this->eventSystem->dispatch<OnMouseMotionEvent>(eventMouse);

                        break;
                    }

                    case SDL_MOUSEWHEEL: {

                        OnMouseWheelEvent eventMouse;

                        eventMouse.x = event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -event.wheel.x : event.wheel.x;
                        eventMouse.y = event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -event.wheel.y : event.wheel.y;
                        eventMouse.preciseX = event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -event.wheel.preciseX : event.wheel.preciseX;
                        eventMouse.preciseY = event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -event.wheel.preciseY : event.wheel.preciseY;

                        this->eventSystem->dispatch<OnMouseWheelEvent>(eventMouse);

                        break;
                    }

                    default:
                        break;
                }
            }


            renderSystem.run();

            return;
        }
        SystemExitEvent event;
        event.exitCode = 0;
        this->eventSystem->dispatch<SystemExitEvent>(event);
    }

    void WindowSystem::end() {
        renderSystem.end();
        logger.info("Finalizando...");
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
} // Monny