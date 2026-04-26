#ifndef __ENGINE_INPUT_H__
#define __ENGINE_INPUT_H__

#include <monny/types.h>
#include <monny/core/config.h>
#include <monny/script/script.h>

typedef struct input_t input_t;

input_t *input_create(keybind_config_t *config, usize count);
void input_destroy(input_t *input);

void input_listener_event(input_t *input);
u8 input_should_quit(input_t *input);

u8 input_key_down(input_t *input, const char *action);
u8 input_key_pressed(input_t *input, const char *action);
u8 input_key_released(input_t *input, const char *action);

#endif