#ifndef __ENGINE_RENDERER_H__
#define __ENGINE_RENDERER_H__

#include <monny/types.h>
#include <monny/window/window.h>

typedef struct renderer_command_t {
    f32 r;
    f32 g;
    f32 b;
} renderer_command_t;

typedef struct renderer_t renderer_t;

renderer_t *renderer_init();
void renderer_destroy(renderer_t *renderer);
void renderer_create_context(renderer_t *renderer, window_t *window);
void renderer_begin(renderer_t *renderer);
void renderer_submit(renderer_t *renderer, renderer_command_t *cmd);
void renderer_end(renderer_t *renderer);

#endif