#include "../../include/LuaEngine/LuaEngine.hpp"

Tasq::LuaEngine::LuaEngine()
{
    logger.info("Inicializando o LuaEngine");
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::table);
}

void Tasq::LuaEngine::loadScript(const std::string &script_path)
{
    logger.info("Carregando o script Lua: {}", script_path);

    sol::function original_print = lua["print"];
    lua["print"] = [](sol::variadic_args args) {};

    sol::load_result script = lua.load_file(script_path);

    if (!script.valid())
    {
        sol::error err = script;
        logger.critical("Erro ao carregar o script Lua: {}", err.what());
        throw std::runtime_error(err.what());
    }

    sol::protected_function_result result = script();

    if (!result.valid())
    {
        sol::error err = result;
        logger.critical("Erro ao executar o script Lua: {}", err.what());
        throw std::runtime_error(err.what());
    }

    lua["print"] = original_print;
}

void Tasq::LuaEngine::registerLoggers()
{
    lua.set_function("log_info", [this](sol::variadic_args args)
                     {
    std::string message;

    for (auto arg : args) {
        message += arg.get_type() == sol::type::string
            ? arg.as<std::string>()
            : arg.get<std::string>();  // tenta converter para string
        message += " ";
    }

    lua_logger.info(message); });

    lua.set_function("log_warn", [this](sol::variadic_args args)
                     {
    std::string message;

    for (auto arg : args) {
        message += arg.get_type() == sol::type::string
            ? arg.as<std::string>()
            : arg.get<std::string>();  // tenta converter para string
        message += " ";
    }

    lua_logger.warn(message); });

    lua.set_function("log_error", [this](sol::variadic_args args)
                     {
    std::string message;

    for (auto arg : args) {
        message += arg.get_type() == sol::type::string
            ? arg.as<std::string>()
            : arg.get<std::string>();  // tenta converter para string
        message += " ";
    }

    lua_logger.error(message); });

    lua.set_function("log_debug", [this](sol::variadic_args args)
                     {
    std::string message;

    for (auto arg : args) {
        message += arg.get_type() == sol::type::string
            ? arg.as<std::string>()
            : arg.get<std::string>();  // tenta converter para string
        message += " ";
    }

    lua_logger.debug(message); });

    lua.set_function("log_critical", [this](sol::variadic_args args)
                     {
    std::string message;

    for (auto arg : args) {
        message += arg.get_type() == sol::type::string
            ? arg.as<std::string>()
            : arg.get<std::string>();  // tenta converter para string
        message += " ";
    }

    lua_logger.critical(message); });
}