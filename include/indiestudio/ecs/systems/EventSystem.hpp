/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems EventSystem.hpp
*/

#pragma once

#include "indiestudio/ecs/Events.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Event;

    //TODO: thread safety omg
    template<typename ManagerType>
    class EventSystem : public BaseSystem<ManagerType> {
    public:
        void process(ManagerType &manager) override {
            manager.template forEntitiesWith<EventCallbacks<ManagerType>>(
                [&manager](auto &data, auto id) {
                    auto &event_manager = manager.getEventManager();
                    const auto &callbacks = manager.template getComponent<EventCallbacks<ManagerType>>(data).getCallbacks();

                    for (const auto &event : event_manager.getEventQueue()) {
                        const auto &callback = callbacks.find(event.first);
                        if (callback != callbacks.end()) {
                            auto funcs = callback->second;
                            for (const auto &func : funcs) {
                                func(event.first, id, manager);
                            }
                        }
                    }
                }
            );
        }

    protected:
    private:
    };
}