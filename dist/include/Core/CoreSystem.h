//
// Created by ricardo on 18/07/25.
//

#ifndef CORESYSTEM_H
#define CORESYSTEM_H

#include <taskflow/taskflow.hpp>
#include "ISystem.h"
#include "Event/EventSystem.h"
#include "Logger/Logger.h"

namespace Monny {

class SystemExitEvent;

class CoreSystem {
    std::unordered_map<std::string, std::shared_ptr<ISystem>> systems;
    std::vector<std::pair<std::string, std::string>> dependencies;
    std::atomic<bool> running {false};

    Logger logger{"CoreSystem"};

    void close(const SystemExitEvent& event);

public:
    CoreSystem() : running(true) { logger.info("Iniciando..."); };
    ~CoreSystem() = default;

    void addSystem(const std::string& name, const std::shared_ptr<ISystem>& system);
    void setDependencie(const std::string& before, const std::string& after);

    template <typename TSystem>
    std::shared_ptr<TSystem> getSystem(const std::string& name) {
        if (systems.count(name)) {
            return std::dynamic_pointer_cast<TSystem>(systems[name]);
        }
        return nullptr;
    }

    void process();
};

} // Monny

#endif //CORESYSTEM_H
