//
// Created by ricardo on 18/07/25.
//

#include "Core/CoreSystem.h"

namespace Monny {

    void CoreSystem::addSystem(const std::string& name, const std::shared_ptr<ISystem>& system) {
        systems[name] = system;
    }

    void CoreSystem::setDependencie(const std::string& before, const std::string& after) {
        dependencies.emplace_back(before, after);
    }

    void CoreSystem::process() {
        tf::Executor executor(std::max<size_t>(1, systems.size()));
        tf::Taskflow taskflow;

        std::unordered_map<std::string, tf::Task> tasks;

        for (auto& [name, system] : systems) {
            if (system) {
                system->name = name;
                system->logger = Logger(name);
                system->eventSystem = getSystem<EventSystem>("EventSystem");

                system->start();

                tasks[name] = taskflow.emplace([&] {
                    system->run();
                });
            }else {
                logger.error("Sistema {} não encontrado", name);
            }
        }

        for (auto& [before, after] : dependencies) {
            if (tasks.count(before) && tasks.count(after)) {
                tasks[before].precede(tasks[after]);
            }
        }

        auto eventSystem = getSystem<EventSystem>("EventSystem");

        eventSystem->listener<SystemExitEvent>([&](const SystemExitEvent& event) {
            close(event);
        });

        while (running) {
            executor.run(taskflow).wait();
            // SystemExitEvent event;
            // eventSystem->dispatch<SystemExitEvent>(event);
        }

    }

    void CoreSystem::close(const SystemExitEvent &event) {
        //std::cout << "saindo com status: " << event.exitCode << '\n';

        for (auto& [name, system] : systems) {
            if (system) {
                system->end();
            }
        }

        logger.info("Saindo com status {}", event.exitCode);
        running = false;
    }
} // Monny