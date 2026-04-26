#include <monny/script/script.h>
#include <monny/core/engine.h>
#include <monny/core/logger.h>

static int l_script_load(script_wt *wt)
{
    const char *path = script_checkstring(wt, 1);

    /* cria env */
    script_newtable(wt);
    int env = script_get_top(wt);

    /* sandbox básico */
    script_newtable(wt);

    script_get_global(wt, "math");
    script_setfield(wt, -2, "math");

    script_get_global(wt, "print");
    script_setfield(wt, -2, "print");

    script_get_global(wt, "pairs");
    script_setfield(wt, -2, "pairs");

    script_get_global(wt, "ipairs");
    script_setfield(wt, -2, "ipairs");

    script_get_global(wt, "input");
    script_setfield(wt, -2, "input");

    script_get_global(wt, "engine");
    script_setfield(wt, -2, "engine");

    script_get_global(wt, "scene");
    script_setfield(wt, -2, "scene");

    script_pushvalue(wt, -1);
    script_setfield(wt, -2, "_G");

    script_newtable(wt);
    script_pushvalue(wt, -2);
    script_setfield(wt, -2, "__index");
    script_setmetatable(wt, env);

    script_pop(wt, 1);

    /* carrega */
    if (script_loadfile(wt, path) != 0)
    {
        script_pushnil(wt);
        return 1;
    }

    if (!script_isfunction(wt, -1))
    {
        script_pushnil(wt);
        return 1;
    }

    /* define environment ANTES de executar */
    script_pushvalue(wt, env);
    script_set_global(wt, "_ENV");

    /* executa chunk */
    if (script_pcall(wt, 0, 1) != SCRIPT_OK)
    {
        script_pushnil(wt);
        return 1;
    }

    /* garante que o retorno existe */
    if (script_get_top(wt) == 0)
    {
        script_pushnil(wt);
        return 1;
    }

    /* retorna valor real do chunk (M) */
    return 1;
}

static void script_script_register(script_t *script)
{
    script_register_function(
        script,
        "script",
        "load",
        l_script_load,
        script);
}

static void script_module_register(script_t *script, engine_t *engine)
{
    (void)engine;
    script_script_register(script);
}

script_module_t script_module = {
    .name = "script",
    .registry_func = script_module_register};

REGISTER_MODULE(script_module)