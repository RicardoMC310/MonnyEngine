//
// Created by ricardo on 17/07/25.
//

#ifndef CORESYSTEM_H
#define CORESYSTEM_H
#include <condition_variable>
#include <queue>
#include <thread>
#include <vector>

#include "ISystem.h"
#include "Logger/Logger.h"

namespace Monny {

class CoreSystem {
    std::queue<std::shared_ptr<ISystem>> systems;
    std::vector<std::thread> threads;
    std::condition_variable cvProcessingSystem;
    std::condition_variable cvFinishedProcess;
    std::mutex mutex;
    std::atomic<int> threadRunning{0};
    std::atomic<bool> running{false};

    Logger logger{"CoreSystem"};

    void run();
    void stop();

public:
    CoreSystem();

    ~CoreSystem();

    template <typename TSystem>
    std::shared_ptr<TSystem> getSystem() {
        std::lock_guard<std::mutex> lock(mutex);
        for (int i = 0 ; i < systems.size(); i++) {
            auto system = systems.front();
            if (auto casted = std::dynamic_pointer_cast<TSystem>(system)) {
                return casted;
            }
        }
        return nullptr;
    }

    void process();
    void addSystem(std::shared_ptr<ISystem> system);
};

} // Monny

#endif //CORESYSTEM_H
