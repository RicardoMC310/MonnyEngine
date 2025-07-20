//
// Created by ricardo on 20/07/25.
//

#include "../../include/Renderer/RenderSystem.h"

#include "Event/EventSystem.h"

namespace Monny {

    void RenderSystem::start() {

        logger.debug("Teste::Testado");
        renderer = SDL_CreateRenderer(
            windowRef,
            -1,
            SDL_RENDERER_ACCELERATED
        );

        if (!renderer) {
            logger.critical("Erro ao criar renderer: {}", SDL_GetError());
        }

        int w, h;

        SDL_GetWindowSize(windowRef, &w, &h);

        frameBuffer.width = w;
        frameBuffer.height = h;
        frameBuffer.texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            frameBuffer.width, frameBuffer.height
        );

        if (!frameBuffer.texture) {
            logger.critical("Erro ao criar frameBuffer: {}", SDL_GetError());
        }

    }

    void RenderSystem::run() {
        // logger.debug("Teste");
        // SDL_SetRenderTarget(renderer, frameBuffer.texture);
        // SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        // SDL_RenderClear(renderer);
        // SDL_SetRenderTarget(renderer, nullptr);

        SDL_Rect rectWindowSize{0, 0, frameBuffer.width, frameBuffer.height};

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
    }

    void RenderSystem::end() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyTexture(frameBuffer.texture);
    }
} // Manny