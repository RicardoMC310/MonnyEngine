#include <monny/script/script.h>
#include <monny/scene/scene_manager.h>

int l_scene_manager_new_scene(script_wt *wt)
{
    scene_manager_t *sm = script_get_context(wt);

    const char *scene_name = script_checkstring(wt, 1);

    handle_t scene_handle = scene_manager_new_scene(sm, scene_name);

    script_pushnumber(wt, scene_handle);
    return 1;
}

void scene_manager_script_register(script_t *script, scene_manager_t *sm)
{
    script_register_function(
        script,
        "scene",
        "new_scene",
        l_scene_manager_new_scene,
        sm);
}