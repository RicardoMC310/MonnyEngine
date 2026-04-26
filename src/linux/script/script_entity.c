#include <monny/script/script.h>
#include <monny/core/engine.h>
#include <monny/core/entity.h>
#include <monny/core/handle.h>

static int l_entity_create(script_wt *wt)
{
    handle_t handle_entity = handle_create(entity_create(), 1);

    script_newtable(wt);
    
    script_pushlightuserdata(wt, (void*)(uiptr)handle_entity);
    script_setfield(wt, -2, "__handle");

    return 1;
}

static void entity_script_register(script_t *script)
{
    script_register_function(
        script,
        "entity",
        "new_entity",
        l_entity_create,
        NULL
    );
}

static void entity_module_register(script_t *script, engine_t *engine)
{
    entity_script_register(script);
}

script_module_t entity_module = {
    .name = "entity",
    .registry_func = entity_module_register
};

REGISTER_MODULE(entity_module)