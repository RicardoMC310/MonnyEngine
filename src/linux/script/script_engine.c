#include <monny/script/script.h>
#include <monny/core/engine.h>

int l_engine_get_fps(script_wt *wt) {
    engine_t *engine = script_get_context(wt);

    script_pushnumber(wt, engine_get_fps(engine));
    return 1;
}

void engine_script_register(script_t *script, engine_t *engine)
{
    script_register_function(
        script, 
        "engine",
        "get_fps",
        l_engine_get_fps,
        engine
    );
}