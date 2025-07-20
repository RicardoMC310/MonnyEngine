//
// Created by ricardo on 20/07/25.
//

#include "../../include/Renderer/RenderSystem.h"

#include "Event/EventSystem.h"

namespace Monny {

    void RenderSystem::start() {
        this->eventSystem->listener<WindowCreatedEvent>([&](const WindowCreatedEvent& event) {
            if (event.window) {
                logger.debug("criando o renderer");
            }
        });
    }

    void RenderSystem::run() {
    }

    void RenderSystem::end() {
    }
} // Manny