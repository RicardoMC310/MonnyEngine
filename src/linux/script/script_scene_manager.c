#include <monny/script/script.h>
#include <monny/scene/scene_manager.h>

int l_scene_manager_new_scene(script_wt *wt)
{
}

void scene_manager_script_register(script_t *script, scene_manager_t *engine)
{
    script_register_function(
        script,
        "scene",
        "new_scene",
        l_scene_manager_new_scene,
        engine);
}