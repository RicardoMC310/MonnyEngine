#include <monny/scene/scene.h>
#include <monny/renderer/renderer.h>

// struct scene_t {

// };

void scene_render(scene_t *scene, renderer_t *renderer)
{
    renderer_command_t cmd = {0};

    cmd.bg_color[0] = 1.0;
    cmd.bg_color[1] = 1.0;
    cmd.bg_color[2] = 0.0;

    renderer_submit(renderer, &cmd);
}