//
// Created by ricardo on 19/07/25.
//
#include "WindowSystem.h"

#include "Event/Events.h"
#include "Event/EventSystem.h"

namespace Monny {
    void WindowSystem::start() {
        logger.info("Iniciando...");

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            logger.critical("Erro ao iniciar o SDL2");
        }

        this->eventSystem->listener<RequestCreateWindowEvent>([&](const RequestCreateWindowEvent& event) {
            window = SDL_CreateWindow(
                event.title.c_str(),
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                event.width, event.height,
                SDL_WINDOW_SHOWN
            );

            if (!window) {
                logger.critical("Erro ao criar janela: {}", SDL_GetError());
            }
        });

    }

    void WindowSystem::run() {
        if (window) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    SystemExitEvent eventExit;
                    this->eventSystem->dispatch<SystemExitEvent>(eventExit);
                }
                OnEventUserInterface eventUser;
                eventUser.event = event;
                this->eventSystem->dispatch<OnEventUserInterface>(eventUser);
            }
        }
    }

    void WindowSystem::end() {
        logger.info("Finalizando...");
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
} // Monny