#ifndef __ENGINE_CONFIG_H__
#define __ENGINE_CONFIG_H__

#define MAX_CHARACTERS_IN_TITLE 64
#define MAX_CHARACTERS_IN_KEYBIND 64
#define MAX_CHARACTERS_IN_VERSION 16

typedef struct keybind_t
{
    char action[MAX_CHARACTERS_IN_KEYBIND];
    char key[MAX_CHARACTERS_IN_KEYBIND];
} keybind_t;

typedef struct window_t {
    int width;
    int height;
    char title[MAX_CHARACTERS_IN_TITLE];
} window_t;

typedef struct engine_config_t {
    window_t window;
    int target_fps;
    keybind_t *keybinds;
    int keybind_count;
    int keybind_size;
    char version[MAX_CHARACTERS_IN_VERSION];
} engine_config_t;

void config_load(engine_config_t *out);

#endif