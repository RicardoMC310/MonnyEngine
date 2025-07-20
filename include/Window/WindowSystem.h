//
// Created by ricardo on 19/07/25.
//

#ifndef WINDOWSYSTEM_H
#define WINDOWSYSTEM_H
#include <SDL2/SDL.h>
#include "Core/ISystem.h"
#include "Renderer/RenderSystem.h"

namespace Monny {

class WindowSystem final : public ISystem {
    SDL_Window* window = nullptr;
    RenderSystem renderSystem;

public:
    WindowSystem() = default;
    ~WindowSystem() = default;

    std::string getMouseButtonName(Uint8 button);

    void start() override;
    void run() override;
    void end() override;

};

} // Monny

#endif //WINDOWSYSTEM_H
