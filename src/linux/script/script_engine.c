#include <monny/script/script.h>
#include <monny/core/engine.h>

static int l_engine_get_fps(script_wt *wt)
{
    engine_t *engine = script_get_context(wt);

    script_pushnumber(wt, engine_get_fps(engine));
    return 1;
}

static void engine_script_register(script_t *script, engine_t *engine)
{
    script_register_function(
        script,
        "engine",
        "get_fps",
        l_engine_get_fps,
        engine);
}

static void engine_module_resgiter(script_t *script, engine_t *engine)
{
    engine_script_register(script, engine);
}

script_module_t engine_module = {
    .name = "engine",
    .registry_func = engine_module_resgiter};

REGISTER_MODULE(engine_module)