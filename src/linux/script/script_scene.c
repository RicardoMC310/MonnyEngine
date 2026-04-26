#include <monny/core/logger.h>
#include <monny/script/script.h>
#include <monny/scene/scene_manager.h>
#include <monny/core/engine.h>

static int l_scene_set_background(script_wt *wt)
{
    scene_manager_t *sm = script_get_context(wt);

    script_getfield(wt, 1, "__handle");
    handle_t scene_handle = (handle_t)(uiptr)script_touserdata(wt, -1);
    script_pop(wt, 1);

    f64 r = script_checknumber(wt, 2);
    f64 g = script_checknumber(wt, 3);
    f64 b = script_checknumber(wt, 4);

    scene_t *scene = scene_manager_get_scene(sm, scene_handle);

    if (!scene)
    {
        LOGGER_ERROR("Scene com o handle %d não encontrada.");
        return 0;
    }
    scene_set_background(scene, (f32)(r / 255), (f32)(g / 255), (f32)(b / 255));

    return 0;
}

static int l_scene_get_name(script_wt *wt)
{
    scene_manager_t *sm = script_get_context(wt);

    script_getfield(wt, 1, "__handle");
    handle_t scene_handle = (handle_t)(uiptr)script_touserdata(wt, -1);
    script_pop(wt, 1);

    scene_t *scene = scene_manager_get_scene(sm, scene_handle);
    if (!scene)
    {
        LOGGER_ERROR("Scene com o handle %d não encontrada.");
        return 0;
    }

    script_pushstring(wt, scene->name);

    return 1;
}

static void scene_script_register(script_t *script, scene_manager_t *sm)
{
    script_register_metatable_function(
        script,
        "Scene",
        "set_background",
        l_scene_set_background,
        sm);

    script_register_metatable_function(
        script,
        "Scene",
        "get_name",
        l_scene_get_name,
        sm);
}

static void scene_module_resgiter(script_t *script, engine_t *engine)
{
    scene_script_register(script, engine->scene_manager);
}

script_module_t scene_module = {
    .name = "scene",
    .registry_func = scene_module_resgiter};

REGISTER_MODULE(scene_module)