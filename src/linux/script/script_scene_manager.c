#include <monny/script/script.h>
#include <monny/scene/scene_manager.h>
#include <monny/core/logger.h>
#include <monny/core/engine.h>

int l_scene_manager_new_scene(script_wt *wt)
{
    scene_manager_t *sm = script_get_context(wt);

    const char *scene_name = script_checkstring(wt, 1);

    handle_t scene_handle = scene_manager_new_scene(sm, scene_name);
    
    script_newtable(wt);

    script_pushlightuserdata(wt, (void*)(uiptr)scene_handle);
    script_setfield(wt, -2, "__handle");

    script_getmetatable(wt, "Scene");
    script_setmetatable(wt, -2);

    return 1;
}

int l_scene_manager_set_current_scene(script_wt *wt)
{
    scene_manager_t *sm = script_get_context(wt);

    script_getfield(wt, 1, "__handle");
    handle_t scene_handle = (handle_t)(uiptr)script_touserdata(wt, -1);
    script_pop(wt, 1);

    scene_manager_swap_current_scene(sm, scene_handle);

    return 0;
}

int l_scene_manager_get_current_scene(script_wt *wt)
{
    scene_manager_t *sm = script_get_context(wt);

    handle_t scene_handle = scene_manager_get_handle_current_scene(sm);

    script_newtable(wt);

    script_pushlightuserdata(wt, (void*)(uiptr)scene_handle);
    script_setfield(wt, -2, "__handle");

    script_getmetatable(wt, "Scene");
    script_setmetatable(wt, -2);

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

    script_register_function(
        script,
        "scene",
        "set_current_scene",
        l_scene_manager_set_current_scene,
        sm);

    script_register_function(
        script,
        "scene",
        "get_current_scene",
        l_scene_manager_get_current_scene,
        sm);
}

static void scene_manager_module_resgiter(script_t *script, engine_t *engine)
{
    scene_manager_script_register(script, engine->scene_manager);
}

script_module_t scene_manager_module = {
    .name = "scene_manager",
    .registry_func = scene_manager_module_resgiter};

REGISTER_MODULE(scene_manager_module)