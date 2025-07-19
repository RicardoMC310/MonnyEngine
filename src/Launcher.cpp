#include <filesystem>

#include "Core/CoreSystem.h"
#include "Event/EventSystem.h"
#include "Logger/Logger.h"
#include "Lua/LuaSystem.h"
#include "Window/WindowSystem.h"
//
// Created by ricardo on 17/07/25.
//

int main(int argc, char** argv) {
    Monny::Logger logger("Launcher");

    if (argc < 2) logger.critical("Obrigatório passar um arquivo .lua");

    if (
        !(std::filesystem::exists(argv[1]) &&
        std::filesystem::is_regular_file(argv[1]) &&
        std::filesystem::path(argv[1]).extension().string() == ".lua")
    ) {
        logger.critical("Arquivo não encontrado ou formato inválido!");
    }

    logger.info("Iniciando...");

    Monny::CoreSystem core;

    core.addSystem("EventSystem", std::make_shared<Monny::EventSystem>());
    core.addSystem("LuaSystem", std::make_shared<Monny::LuaSystem>(argv[1]));
    core.addSystem("WindowSystem", std::make_shared<Monny::WindowSystem>());

    core.setDependencie("LuaSystem", "WindowSystem");

    core.process();

    logger.info("Finalizando...");

    return 0;
}