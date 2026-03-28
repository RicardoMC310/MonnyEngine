#include <monny/script/script.h>
#include <monny/core/logger.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// script_call_ret(L, ref, "dd>d", 2.0, 3.0, &result); possivel add

struct script_t
{
    lua_State *L;
    int ref_setup;
    int ref_update;
    int ref_stop;
};

struct script_wt {
    lua_State *L;
    void *ctx;
};

static int script_mock_print(lua_State *L);
int script_load_ref_function(script_t *script, const char *func_name);
void script_call_ref_function(script_t *script, int ref, const char *sig, ...);
static int script_dispatch(lua_State *L);

script_t *script_create(const char *filepath)
{
    script_t *script = malloc(sizeof(script_t));
    if (!script)
    {
        perror("malloc script");
        return NULL;
    }

    script->L = luaL_newstate();
    luaL_openlibs(script->L);

    // lua_pushcfunction(script->L, script_mock_print);
    // lua_setglobal(script->L, "print");

    if (luaL_dofile(script->L, filepath) != LUA_OK)
    {
        LOGGER_ERROR("%s", lua_tostring(script->L, -1));
        script_destroy(script);
        return NULL;
    }

    script->ref_setup = script_load_ref_function(script, "onSetup");
    script->ref_update = script_load_ref_function(script, "onUpdate");
    script->ref_stop = script_load_ref_function(script, "onStop");

    script_call_ref_function(script, script->ref_setup, "");

    return script;
}

void script_destroy(script_t *script)
{
    if (!script)
        return;

    script_call_ref_function(script, script->ref_stop, "");

    lua_close(script->L);
    free(script);
    script = NULL;
}

void script_update(script_t *script, double deltaTime)
{
    if (!script)
        return;

    script_call_ref_function(script, script->ref_update, "d", deltaTime);
}

void script_register_function(
    script_t *script,
    const char *table,
    const char *name,
    script_func func,
    void *ctx)
{
     lua_State *L = script->L;

    // cria tabela se não existir
    lua_getglobal(L, table);
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        lua_newtable(L);
        lua_setglobal(L, table);
        lua_getglobal(L, table);
    }

    // stack: [tabela]

    lua_pushlightuserdata(L, func);
    lua_pushlightuserdata(L, ctx);
    lua_pushcclosure(L, script_dispatch, 2);

    lua_setfield(L, -2, name);

    lua_pop(L, 1); // remove tabela
}

const char *script_checkstring(script_wt *wt, int index)
{
    return luaL_checkstring(wt->L, index);
}

int script_checkint(script_wt *wt, int index)
{
    return luaL_checkinteger(wt->L, index);
}

void script_pushboolean(script_wt *wt, int value)
{
    lua_pushboolean(wt->L, value);
}

void *script_get_context(script_wt *wt)
{
    return wt->ctx;
}

int script_load_ref_function(script_t *script, const char *func_name)
{
    lua_getglobal(script->L, func_name);

    if (!lua_isfunction(script->L, -1))
    {
        LOGGER_WARN("%s não é uma função", func_name);
        return LUA_REFNIL;
    }

    return luaL_ref(script->L, LUA_REGISTRYINDEX);
}

void script_call_ref_function(script_t *script, int ref, const char *sig, ...)
{
    va_list args;
    va_start(args, sig);

    lua_rawgeti(script->L, LUA_REGISTRYINDEX, ref);

    int nargs = 0;

    for (const char *p = sig; *p; p++)
    {

        switch (*p)
        {
        case 'd':
        {
            double d = va_arg(args, double);
            lua_pushnumber(script->L, d);
            break;
        }
        case 'i':
        {
            int i = va_arg(args, int);
            lua_pushinteger(script->L, i);
            break;
        }
        case 's':
        {
            const char *s = va_arg(args, const char *);
            lua_pushstring(script->L, s);
            break;
        }
        case 'b':
        {
            int b = va_arg(args, int);
            lua_pushboolean(script->L, b);
            break;
        }
        default:
            LOGGER_ERROR("Tipo inválido: %c", *p);
            va_end(args);
            return;
        }

        nargs++;
    }

    if (lua_pcall(script->L, nargs, 0, 0) != LUA_OK)
    {
        LOGGER_ERROR("Erro: %s", lua_tostring(script->L, -1));
    }

    va_end(args);
}

static int script_mock_print(lua_State *L)
{
    (void)L;
    return 0;
}

static int script_dispatch(lua_State *L)
{
    script_func func =
        (script_func)lua_touserdata(L, lua_upvalueindex(1));

    void *ctx = lua_touserdata(L, lua_upvalueindex(2));

    script_wt wt = {
        .L = L,
        .ctx = ctx
    };

    return func(&wt);
}