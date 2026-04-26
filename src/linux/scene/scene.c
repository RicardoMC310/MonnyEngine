#include <monny/scene/scene.h>
#include <monny/renderer/renderer.h>

#include <stdio.h>
#include <stdlib.h>

// struct scene_t {

// };

void scene_render(scene_t *scene, renderer_t *renderer)
{
    renderer_command_t cmd;
    // if (!cmd) return;

    cmd.r = scene->r;
    cmd.g = scene->g;
    cmd.b = scene->b;

    renderer_submit(renderer, &cmd);
}

void scene_set_background(scene_t *scene, f32 r, f32 g, f32 b)
{
    if (!scene) return;

    scene->r = r;
    scene->g = g;
    scene->b = b;
}