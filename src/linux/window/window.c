#include "monny/window/window.h"
#include "monny/core/logger.h"

#include "SDL3/SDL.h"

#include <stdlib.h>
#include <stdbool.h>

struct window_t
{
    SDL_Window *sdlWindow;
    int shouldClose;
};

int init_window_main_system();

static int number_of_the_window = 0;

window_t *window_create(window_config_t *window_config)
{
    window_t *window = malloc(sizeof(window_t));
    if (!window)
    {
        LOGGER_ERROR("Erro ao alocar buffer de janela");
        return NULL;
    }

    int success_init_main_system = 0;
    if (number_of_the_window == 0)
    {
        success_init_main_system = init_window_main_system();
    }

    if (success_init_main_system != 0)
    {
        free(window);
        return NULL;
    }

    window->sdlWindow = SDL_CreateWindow(
        window_config->title,
        window_config->width,
        window_config->height,
        0);
    if (!window->sdlWindow)
    {
        free(window);
        LOGGER_ERROR("Erro ao criar janela");
        return NULL;
    }

    number_of_the_window++;

    return window;
}

void window_destroy(window_t *window)
{
    if (!window)
        return;

    SDL_DestroyWindow(window->sdlWindow);
    number_of_the_window--;

    if (number_of_the_window <= 0)
    {
        SDL_Quit();
    }

    free(window);
    window = NULL;
}

int init_window_main_system()
{
    int success_init = SDL_Init(SDL_INIT_VIDEO);
    if (!success_init)
    {
        LOGGER_ERROR("Erro ao iniciar sistema de janelas");
        return 1;
    }

    return 0;
}