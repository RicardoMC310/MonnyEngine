#include <monny/renderer/renderer.h>
#include <monny/renderer/renderer_backend.h>
#include <monny/core/logger.h>

#include <glad/glad.h>
#include <SDL3/SDL.h>

#include <stdio.h>
#include <stdlib.h>

struct renderer_t
{
    window_t *window;
    SDL_GLContext context;

    renderer_command_t *g_commands;
    usize g_count;
    usize g_capacity;
};

renderer_t *renderer_init()
{
    renderer_t *renderer = malloc(sizeof(renderer_t));
    if (!renderer)
    {
        perror("malloc renderer");
        return NULL;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

#ifdef DEBUG
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    renderer->g_capacity = 1024;
    renderer->g_commands = calloc(renderer->g_capacity, sizeof(renderer_command_t));
    if (!renderer->g_commands)
    {
        perror("calloc g_commands");
        free(renderer);
        return NULL;
    }
    renderer->g_count = 0;

    return renderer;
}

void renderer_destroy(renderer_t *renderer)
{
    if (!renderer)
        return;

    if (renderer->g_commands)
        free(renderer->g_commands);

    if (renderer->context)
        SDL_GL_DestroyContext(renderer->context);

    free(renderer);
    renderer = NULL;
}

void renderer_create_context(renderer_t *renderer, window_t *window)
{
    if (!renderer || !window)
        return;

    renderer->window = window;

    renderer->context = SDL_GL_CreateContext(window_get_raw_window(window));
    if (!renderer->context)
    {
        LOGGER_ERROR("Erro ao inicializar contexto");
        return;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        LOGGER_ERROR("Erro ao inicializar glad");
    }
}

void renderer_begin(renderer_t *renderer)
{
    if (!renderer)
        return;

    renderer->g_count = 0;
}

void renderer_submit(renderer_t *renderer, renderer_command_t *cmd)
{
    if (!renderer || !cmd)
        return;

    if (renderer->g_count >= renderer->g_capacity)
    {
        usize new_capacity = renderer->g_capacity * 2;
        renderer_command_t *new_cmds = realloc(renderer->g_commands, new_capacity * sizeof(renderer_command_t));
        if (!new_cmds)
            return;
        renderer->g_commands = new_cmds;
        renderer->g_capacity = new_capacity;
    }

    // renderer->g_commands[renderer->g_count++] = *cmd;
    memcpy(&renderer->g_commands[renderer->g_count++], cmd, sizeof(*cmd));
}

void renderer_end(renderer_t *renderer)
{
    if (!renderer)
        return;

    for (usize i = 0; i < renderer->g_count; i++) {
        renderer_command_t *cmd = &renderer->g_commands[i];

        glClearColor(cmd->r, cmd->g, cmd->b, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    SDL_GL_SwapWindow(window_get_raw_window(renderer->window));
}