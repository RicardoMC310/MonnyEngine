#include <monny/input/input.h>
#include <monny/helpers/hash_string.h>
#include <monny/core/logger.h>

#include <SDL3/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct keybind_t
{
    char *action;
    u32 action_hash;
    SDL_Scancode scancode;
};

struct input_t
{
    struct keybind_t *keybinds;
    usize keybind_count;

    u8 keys[SDL_SCANCODE_COUNT];
    u8 keys_pressed[SDL_SCANCODE_COUNT];

    u8 should_quit;
};

SDL_Scancode keycode_from_string(const char *key);

input_t *input_create(keybind_config_t *config, usize count)
{
    input_t *input = calloc(1, sizeof(input_t));
    if (!input)
    {
        perror("malloc input");
        return NULL;
    }

    input->should_quit = false;

    input->keybinds = calloc(count, sizeof(struct keybind_t));
    input->keybind_count = count;
    for (usize i = 0; i < count; i++)
    {
        input->keybinds[i] = (struct keybind_t){
            .action_hash = hash_string(config[i].action),
            .action = strdup(config[i].action),
            .scancode = keycode_from_string(config[i].key)};
    }

    return input;
}

void input_destroy(input_t *input)
{
    if (!input)
        return;

    if (input->keybinds)
    {
        for (usize i = 0; i < input->keybind_count; i++)
        {
            free(input->keybinds[i].action);
            input->keybinds[i].action = NULL;
        }

        free(input->keybinds);
        input->keybinds = NULL;
    }

    free(input);
    input = NULL;
}

void input_listener_event(input_t *input)
{
    SDL_Event event;

    memset(input->keys_pressed, 0, sizeof(input->keys_pressed));

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            input->should_quit = true;
            break;
        case SDL_EVENT_KEY_DOWN:
        {
            SDL_Scancode sc = event.key.scancode;

            if (!input->keys[sc])
                input->keys_pressed[sc] = 1;

            input->keys[sc] = 1;

            break;
        }
        case SDL_EVENT_KEY_UP:
        {
            SDL_Scancode sc = event.key.scancode;
            input->keys[sc] = 0;
            break;
        }
        }
    }
}

u8 input_should_quit(input_t *input)
{
    if (!input)
        return true;

    return input->should_quit;
}

u8 input_keydown(input_t *input, const char *action)
{
    u32 hash = hash_string(action);

    for (usize i = 0; i < input->keybind_count; i++)
    {
        if (input->keybinds[i].action_hash == hash && strcmp(input->keybinds[i].action, action) == 0)
        {
            return input->keys[input->keybinds[i].scancode];
        }
    }

    return 0;
}

SDL_Scancode keycode_from_string(const char *key)
{
    SDL_Keycode kc = SDL_GetKeyFromName(key);

    if (kc == SDLK_UNKNOWN)
    {
        LOGGER_WARN("Tecla inválida: %s", key);
        return SDL_SCANCODE_UNKNOWN;
    }

    return SDL_GetScancodeFromKey(kc, NULL);
}