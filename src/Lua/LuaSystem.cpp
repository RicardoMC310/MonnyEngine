//
// Created by ricardo on 18/07/25.
//

#include "../../include/Lua/LuaSystem.h"

#include <SDL_events.h>
#include <sstream>
#include <utility>

#include "Event/Events.h"
#include "Event/EventSystem.h"

namespace Monny {

    void LuaSystem::setNewUserTable() {
        lua.new_usertype<Logger>("Logger",
            sol::constructors<Logger(std::string)>(),

            "info", [&](Logger& self, sol::variadic_args args) {
                std::ostringstream oss;

                for (const auto& arg : args) {
                    oss << arg.as<std::string>();
                }

                self.info(oss.str());
            },

            "warn", [&](Logger& self, sol::variadic_args args) {
                std::ostringstream oss;

                for (const auto& arg : args) {
                    oss << arg.as<std::string>();
                }

                self.warn(oss.str());
            },

            "error", [&](Logger& self, sol::variadic_args args) {
                std::ostringstream oss;

                for (const auto& arg : args) {
                    oss << arg.as<std::string>();
                }

                self.error(oss.str());
            },

            "critical", [&](Logger& self, sol::variadic_args args) {
                std::ostringstream oss;

                for (const auto& arg : args) {
                    oss << arg.as<std::string>();
                }

                self.critical(oss.str());
            },

            "debug", [&](Logger& self, sol::variadic_args args) {
                std::ostringstream oss;

                for (const auto& arg : args) {
                    oss << arg.as<std::string>();
                }

                self.debug(oss.str());
            },

            "trace", [&](Logger& self, sol::variadic_args args) {
                std::ostringstream oss;

                for (const auto& arg : args) {
                    oss << arg.as<std::string>();
                }

                self.trace(oss.str());
            }
        );

        lua["logger"] = &this->luaLogger;
    }

    void LuaSystem::createFunctions() {
        lua.set_function("createWindow", [&](std::string title, uint width, uint height) {
            RequestCreateWindowEvent requestEvent;

            requestEvent.title = std::move(title);
            requestEvent.width = width;
            requestEvent.height = height;

            this->eventSystem->dispatch<RequestCreateWindowEvent>(requestEvent);
        });

        lua.set_function("isKeyDown", [&](std::string key)-> bool {
            auto keyboardState = SDL_GetKeyboardState(nullptr);

            auto scanCode = SDL_GetScancodeFromName(key.c_str());

            if (scanCode == SDL_SCANCODE_UNKNOWN) {
                logger.error("Tecla não encontrada.");
                return false;
            }

            return keyboardState[scanCode];
        });

        lua.set_function("exit", [&](int code) {
            SystemExitEvent event;
            event.exitCode = code;
            this->eventSystem->dispatch<SystemExitEvent>(event);
        });
    }

    void LuaSystem::onDistribitionUserEvents() {
        this->eventSystem->listener<OnEventUserInterface>([&](const OnEventUserInterface& event) {
            if (event.event.type == SDL_KEYDOWN) {
                lua["onKeyDown"](SDL_GetKeyName(event.event.key.keysym.sym));
            }

            if (event.event.type == SDL_KEYUP) {
                lua["onKeyUp"](SDL_GetKeyName(event.event.key.keysym.sym));
            }
        });
    }

    void LuaSystem::start() {
        this->logger.info("Iniciando...");

        lua.open_libraries();

        setNewUserTable();
        createFunctions();

        lua.script_file(pathLuaFile);

        std::vector<std::string> functionsName = {"onSetup", "onUpdate", "onDestroy"};

        for (auto& functionName : functionsName) {
            auto func = lua[functionName];

            if (func == sol::nil) {
                logger.critical("Função {} não encontrada!", functionName);
            }
        }

        sol::protected_function_result result = lua["onSetup"]();

        if (!result.valid()) {
            sol::error err = result;
            logger.critical("{}", err.what());
        }

        onDistribitionUserEvents();

    }

    void LuaSystem::run() {
        sol::protected_function_result result = lua["onUpdate"](1);

        if (!result.valid()) {
            sol::error err = result;
            logger.critical("{}", err.what());
        }
    }

    void LuaSystem::end() {
        sol::protected_function_result result = lua["onDestroy"]();

        if (!result.valid()) {
            sol::error err = result;
            logger.critical("{}", err.what());
        }
    }
} // Monny