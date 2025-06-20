#include "../../include/Engine/Engine.hpp"

Tasq::Engine::Engine()
{
    logger.info("Inicializando a Engine.");
    isRunning = true;
}

void Tasq::Engine::runArchive(const std::string &archivePath)
{
    logger.info("Executando o arquivo de script: {}", archivePath);

    try
    {
        luaEngine.registerLoggers();
        luaEngine.loadScript(archivePath);

        sol::state& lua = luaEngine.getLuaState();

        window.init(&lua);
        window.registerFuncs();
    }
    catch (const std::exception &e)
    {
        return;
    }
}

void Tasq::Engine::run()
{
    logger.info("Iniciando a execução da Engine.");

    sol::state& lua = luaEngine.getLuaState();

    startFunction = lua["start"];
    if (!startFunction.valid()) logger.critical("Função 'start' não encontrada no script Lua.");

    updateFunction = lua["update"];
    if (!updateFunction.valid()) logger.critical("Função 'update' não encontrada no script Lua.");

    cleanup = lua["cleanup"];
    if (!cleanup.valid()) logger.critical("Função 'cleanup' não encontrada no script Lua.");

    logger.info("Engine em execução.");

    sol::protected_function_result result = startFunction();
    if (!result.valid())
    {
        sol::error err = result;
        logger.critical("Erro ao executar a função 'start': {}", err.what());
    }

    while (isRunning)
    {
        result = updateFunction();
        if (!result.valid())
        {
            sol::error err = result;
            logger.critical("Erro ao executar a função 'update': {}", err.what());
            break;
        }
    }

    result = cleanup();
    if (!result.valid())
    {
        sol::error err = result;
        logger.critical("Erro ao executar a função 'cleanup': {}", err.what());
    }
}

void Tasq::Engine::registerFuncs()
{
    sol::state& lua = luaEngine.getLuaState();

    lua.set_function("quit", [this]() {
        logger.info("Função 'quit' chamada. Encerrando a Engine.");
        isRunning = false;
    });
}