#include <monny/scene/scene_manager.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct scene_manager_t
{
    handle_t scene_current;

    scene_t *scenes;
    u32 scenes_count;
    u32 scenes_capacity;
};

scene_manager_t *scene_manager_create()
{
    scene_manager_t *sm = malloc(sizeof(scene_manager_t));
    if (!sm)
    {
        perror("malloc scene manager");
        return NULL;
    }

    sm->scene_current = 0;
    sm->scenes_capacity = 8;
    sm->scenes_count = 0;
    sm->scenes = calloc(sm->scenes_capacity, sizeof(scene_t));
    if (!sm->scenes)
    {
        perror("calloc scene manager scenes");
        scene_manager_destroy(sm);
        return NULL;
    }

    return sm;
}

void scene_manager_destroy(scene_manager_t *sm)
{
    if (!sm)
        return;

    if (sm->scenes)
    {
        free(sm->scenes);
    }

    free(sm);
    sm = NULL;
}

handle_t scene_manager_new_scene(scene_manager_t *sm, const char *name)
{
    if (!sm)
        return handle_create(0, 0);

    if (sm->scenes_count >= sm->scenes_capacity)
    {
        sm->scenes_capacity *= 2;
        sm->scenes = realloc(sm->scenes, sizeof(scene_t) * sm->scenes_capacity);
    }

    handle_t handle = handle_create(sm->scenes_count++, 1);

    sm->scenes[handle_get_index(handle)] = (scene_t){
        .r = 1,
        .g = 1,
        .b = 0};
    strncpy(sm->scenes[handle_get_index(handle)].name, name, 63);
    sm->scenes[handle_get_index(handle)].name[63] = '\0';

    return handle;
}

scene_t *scene_manager_get_current_scene(scene_manager_t *sm)
{
    if (!sm)
        return NULL;
    return &sm->scenes[handle_get_index(sm->scene_current)];
}

void scene_manager_swap_current_scene(scene_manager_t *sm, handle_t handle_scene)
{
    if (!sm)
        return;
    sm->scene_current = handle_scene;
}