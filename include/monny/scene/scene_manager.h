#ifndef __ENGINE_SCENE_MANAGER_H__
#define __ENGINE_SCENE_MANAGER_H__

#include <monny/types.h>
#include <monny/core/handle.h>
#include <monny/script/script.h>
#include <monny/scene/scene.h>

typedef struct scene_manager_t scene_manager_t;

scene_manager_t *scene_manager_create();
void scene_manager_destroy(scene_manager_t *sm);

handle_t scene_manager_new_scene(scene_manager_t *sm, const char *name);
scene_t *scene_manager_get_current_scene(scene_manager_t *sm);
scene_t *scene_manager_get_scene(scene_manager_t *sm, handle_t handle_scene);
void scene_manager_swap_current_scene(scene_manager_t *sm, handle_t handle_scene);

void scene_manager_script_register(script_t *script, scene_manager_t *engine);

#endif