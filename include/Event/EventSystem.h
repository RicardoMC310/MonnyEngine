//
// Created by ricardo on 18/07/25.
//

#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <sol/protected_function.hpp>

#include "Events.h"
#include "Core/CoreSystem.h"
#include "Core/ISystem.h"

namespace Monny {

class EventSystem final : public ISystem {
    using Listener = std::function<void(const Event&)>;
    std::unordered_map<std::type_index, std::vector<Listener>> events;

public:
    EventSystem() = default;
    ~EventSystem() = default;

    template <typename TEvent>
    void listener(std::function<void(const TEvent&)> func) {
        static_assert(std::is_base_of<Event, TEvent>::value, "Evento tem q derivar de Monny::Event");

        std::type_index type = typeid(TEvent);
        events[type].emplace_back([func = std::move(func)](const Event& event) {
            func(static_cast<const TEvent&>(event));
        });
    }

    template <typename TEvent>
    void dispatch(Event& event) {
        static_assert(std::is_base_of<Event, TEvent>::value, "Evento tem q derivar de Monny::Event");

        std::type_index index = typeid(TEvent);
        auto it = events.find(index);
        if (it != events.end()) {
            for (const auto& func : it->second) {
                func(event); // chama todos os listeners registrados
            }
        }
    }

    void run() override;
    void start() override;
    void end() override;
};

} // Monny

#endif //EVENTSYSTEM_H
