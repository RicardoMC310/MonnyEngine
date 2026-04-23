#include <monny/scene/scene.h>
#include <monny/renderer/renderer.h>

// struct scene_t {

// };

void scene_render(scene_t *scene, renderer_t *renderer)
{
    renderer_command_t cmd = {0};

    cmd.bg_color[0] = scene->r;
    cmd.bg_color[1] = scene->g;
    cmd.bg_color[2] = scene->b;

    renderer_submit(renderer, &cmd);
}