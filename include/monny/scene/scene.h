#ifndef __ENGINE_SCENE_H__
#define __ENGINE_SCENE_H__

#include <monny/types.h>
#include <monny/renderer/renderer.h>

typedef struct scene_t {} scene_t;

void scene_render(scene_t *scene, renderer_t *renderer);

#endif