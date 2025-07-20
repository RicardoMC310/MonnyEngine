//
// Created by ricardo on 20/07/25.
//

#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include "Core/ISystem.h"

namespace Monny {

class RenderSystem final : public ISystem {
public:
    void start() override;
    void run() override;
    void end() override;
};

} // Manny

#endif //RENDERSYSTEM_H
