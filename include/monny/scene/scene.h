#ifndef __ENGINE_SCENE_H__
#define __ENGINE_SCENE_H__

#include <monny/types.h>
#include <monny/renderer/renderer.h>
#include <monny/script/script.h>

typedef struct scene_manager_t scene_manager_t;

typedef struct scene_t
{
    char name[64];
    f32 r, g, b;
} scene_t;

void scene_render(scene_t *scene, renderer_t *renderer);
void scene_set_background(scene_t *scene, f32 r, f32 g, f32 b);

#endif