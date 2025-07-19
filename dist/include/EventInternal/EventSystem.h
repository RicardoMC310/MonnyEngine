//
// Created by ricardo on 17/07/25.
//

#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H
#include <iostream>
#include "CoreSystem/ISystem.h"
#include "Logger/Logger.h"
#include <memory>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <map>
#include <typeindex>

namespace Monny {

class Event {
    public:
        virtual std::string getName() const {return "Event";};
};

class EventSystem : public ISystem {
    std::queue<std::shared_ptr<Event>> eventsQueue;
    std::atomic<bool> running{false};
    Logger logger{"EventSystem"};
    std::mutex mutex;
    std::map<std::type_index, std::vector<std::function<void(std::shared_ptr<Event>)>>> events;
    std::condition_variable cvProcessingEvent;

public:
    EventSystem();
    ~EventSystem();

    void Publish(std::shared_ptr<Event> event);

    template <typename TEvent>
    void Subscribe(std::function<void(std::shared_ptr<TEvent>)> callback) {
        std::type_index type = typeid(TEvent);

        events[type].push_back(
            [callback](std::shared_ptr<Event> event) {
                if (auto typed_event = std::dynamic_pointer_cast<TEvent>(event)) {
                    callback(typed_event);
                }
            }
        );

    }

    void run() override;
};

} // Monny

#endif //EVENTSYSTEM_H
