//
// Created by ricardo on 19/07/25.
//

#ifndef WINDOWSYSTEM_H
#define WINDOWSYSTEM_H
#include <SDL2/SDL.h>
#include "Core/ISystem.h"

namespace Monny {

class WindowSystem final : public ISystem {
    SDL_Window* window = nullptr;

public:
    WindowSystem() = default;
    ~WindowSystem() = default;

    void start() override;
    void run() override;
    void end() override;

};

} // Monny

#endif //WINDOWSYSTEM_H
