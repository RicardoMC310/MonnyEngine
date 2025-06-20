#include "../../include/Window/Window.hpp"

Tasq::Window::Window()
{
    int result = SDL_Init(SDL_INIT_VIDEO);
    if (result < 0)
    {
        const char *err = SDL_GetError();
        logger.critical("Falha ao inicializar o SDL: '{}' (code: {})", err && *err ? err : "sem mensagem", result);
    }

    window.reset();
}

void Tasq::Window::create(const std::string &title, int width, int height)
{
    logger.info("Criando a janela: '{}' ({}, {})", title, width, height);

    window = std::shared_ptr<SDL_Window>(
        SDL_CreateWindow(title.c_str(), width, height, 0),
        [](SDL_Window *w)
        { SDL_DestroyWindow(w); });
    if (!window)
    {
        const char *err = SDL_GetError();
        logger.critical("Falha ao criar a janela: '{}' (code: {})", err && *err ? err : "sem mensagem", SDL_GetError());
    }
}

void Tasq::Window::registerFuncs()
{
    if (!lua)
    {
        logger.critical("Lua state não inicializado.");
        return;
    }

    lua->set_function("createWindow", [this](const std::string &title, int width, int height)
                      { create(title, width, height); });

    lua->set_function("getWindowSize", [this]() -> sol::table
                      {
        if (!window)
        {
            logger.error("Janela não criada.");
            return sol::nil;
        }

        int width, height;
        SDL_GetWindowSize(window.get(), &width, &height);
        return lua->create_table_with(
            "width", width,
            "height", height
        ); });

    lua->set_function("destroyWindow", [this]()
                      {
        if (!window)
        {
            logger.error("Janela não criada.");
            return;
        }

        SDL_DestroyWindow(window.get());
        window.reset();
        logger.info("Janela destruída."); });
}