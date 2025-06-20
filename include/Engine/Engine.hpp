#pragma once

#include "../../include/Logger/Logger.hpp"

namespace Tasq
{
    class Engine
    {
    private:
        Logger logger{"Engine"};
    public:
        Engine();
        ~Engine() = default;
    };
}