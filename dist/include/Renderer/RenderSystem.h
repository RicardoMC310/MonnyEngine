//
// Created by ricardo on 20/07/25.
//

#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include <SDL2/SDL.h>

#include "Core/ISystem.h"

namespace Monny {

    struct FrameBuffer {
        SDL_Texture* texture = nullptr;
        int width = 0, height = 0;
    };

    class RenderSystem final : public SubSystem {
        FrameBuffer frameBuffer;
        SDL_Renderer* renderer = nullptr;

    public:
        SDL_Window* windowRef;

        RenderSystem(): renderer(nullptr) {}
        ~RenderSystem() = default;

        void start() override;
        void run() override;
        void end() override;
    };

} // Manny

#endif //RENDERSYSTEM_H
