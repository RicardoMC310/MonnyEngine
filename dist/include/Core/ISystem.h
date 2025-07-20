//
// Created by ricardo on 18/07/25.
//

#ifndef ISYSTEM_H
#define ISYSTEM_H
#include <memory>

#include "Logger/Logger.h"

namespace Monny {

    class EventSystem;

    class ISystem {
    public:
        virtual ~ISystem() = default;

        std::shared_ptr<EventSystem> eventSystem = nullptr;
        std::string name;
        Logger logger{"" + std::to_string(std::chrono::high_resolution_clock::now().time_since_epoch().count())};
        virtual void run() = 0;
        virtual void start() = 0;
        virtual void end() = 0;
    };

    class SubSystem : public ISystem {
    public:
    };

}

#endif //ISYSTEM_H
