#include <monny/script/script.h>
#include <monny/core/logger.h>
#include <monny/types.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// script_call_ret(L, ref, "dd>d", 2.0, 3.0, &result); possivel add

extern script_module_t *__start_modules;
extern script_module_t *__stop_modules;

struct script_t
{
    lua_State *L;
};

struct script_wt
{
    lua_State *L;
    void *ctx;
};

static int script_mock_print(lua_State *L);
static int script_dispatch(lua_State *L);
static int script_traceback(lua_State *L);

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

    // script->ref_setup = script_load_ref_function(script, "onSetup");
    // script->ref_update = script_load_ref_function(script, "onUpdate");
    // script->ref_stop = script_load_ref_function(script, "onStop");

    // script_call_ref_function(script, script->ref_setup, "");

    return script;
}

void script_destroy(script_t *script)
{
    if (!script)
        return;

    // script_call_ref_function(script, script->ref_stop, "");

    lua_close(script->L);
    free(script);
    script = NULL;
}

void script_init_modules(script_t *script, engine_t *ctx)
{
    if (!script || !ctx)
        return;

    for (script_module_t **mod = &__start_modules;
         mod < &__stop_modules;
         mod++)
    {
        if (*mod && (*mod)->registry_func)
        {
            (*mod)->registry_func(script, ctx);
        }
    }
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
    if (!lua_istable(L, -1))
    {
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

void script_register_metatable_function(
    script_t *script,
    const char *metatable,
    const char *name,
    script_func func,
    void *ctx)
{
    lua_State *L = script->L;

    // cria ou pega metatable
    if (luaL_newmetatable(L, metatable))
    {
        // cria table de métodos
        lua_newtable(L); // methods table

        // registra função
        lua_pushlightuserdata(L, func);
        lua_pushlightuserdata(L, ctx);
        lua_pushcclosure(L, script_dispatch, 2);
        lua_setfield(L, -2, name);

        // salva methods table como __index
        lua_setfield(L, -2, "__index");

        // dup metatable e registra __metatable lock (opcional)
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__metatable");
    }
    else
    {
        // metatable já existe → pega __index table
        lua_getfield(L, -1, "__index");

        if (!lua_istable(L, -1))
        {
            lua_pop(L, 1);
            lua_newtable(L);
        }

        lua_pushlightuserdata(L, func);
        lua_pushlightuserdata(L, ctx);
        lua_pushcclosure(L, script_dispatch, 2);
        lua_setfield(L, -2, name);

        lua_setfield(L, -2, "__index");
    }

    lua_pop(L, 1);
}

const char *script_checkstring(script_wt *wt, int index)
{
    return luaL_checkstring(wt->L, index);
}

int script_checkint(script_wt *wt, int index)
{
    return luaL_checkinteger(wt->L, index);
}

double script_checknumber(script_wt *wt, int index)
{
    return luaL_checknumber(wt->L, index);
}

void *script_touserdata(script_wt *wt, int index)
{
    return lua_touserdata(wt->L, index);
}

void script_pushboolean(script_wt *wt, int value)
{
    lua_pushboolean(wt->L, value);
}

void script_pushnil(script_wt *wt)
{
    lua_pushnil(wt->L);
}

const char *script_tostring(script_wt *wt, int index)
{
    return lua_tostring(wt->L, index);
}

void script_pushnumber(script_wt *wt, double value)
{
    lua_pushnumber(wt->L, value);
}

void script_pushstring(script_wt *wt, const char *value)
{
    lua_pushstring(wt->L, value);
}

void script_pushlightuserdata(script_wt *wt, void *ptr)
{
    lua_pushlightuserdata(wt->L, ptr);
}

void script_getmetatable(script_wt *wt, const char *name)
{
    luaL_getmetatable(wt->L, name);
}

void script_setmetatable(script_wt *wt, int index)
{
    lua_setmetatable(wt->L, index);
}

void script_pop(script_wt *wt, int n)
{
    lua_pop(wt->L, n);
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
    lua_State *L = script->L;

    va_list args;
    va_start(args, sig);

    // 1. push error handler
    lua_pushcfunction(L, script_traceback);
    int errfunc = lua_gettop(L);

    // 2. push função
    lua_rawgeti(L, LUA_REGISTRYINDEX, ref);

    int nargs = 0;

    // 3. push args
    for (const char *p = sig; *p; p++)
    {
        switch (*p)
        {
        case 'd':
        {
            double d = va_arg(args, double);
            lua_pushnumber(L, d);
            break;
        }
        case 'i':
        {
            int i = va_arg(args, int);
            lua_pushinteger(L, i);
            break;
        }
        case 's':
        {
            const char *s = va_arg(args, const char *);
            lua_pushstring(L, s);
            break;
        }
        case 'b':
        {
            int b = va_arg(args, int);
            lua_pushboolean(L, b);
            break;
        }
        default:
            LOGGER_ERROR("Tipo inválido: %c", *p);
            va_end(args);
            lua_pop(L, 1); // remove error handler
            return;
        }

        nargs++;
    }

    // 4. chama com error handler
    if (lua_pcall(L, nargs, 0, errfunc) != LUA_OK)
    {
        LOGGER_ERROR("%s", lua_tostring(L, -1));
        lua_pop(L, 1); // remove erro
    }

    // 5. remove error handler
    lua_remove(L, errfunc);

    va_end(args);
}

void script_newtable(script_wt *wt)
{
    lua_newtable(wt->L);
}

void script_setfield(script_wt *wt, int index, const char *key)
{
    lua_setfield(wt->L, index, key);
}

void script_getfield(script_wt *wt, int index, const char *key)
{
    lua_getfield(wt->L, index, key);
}

void script_pushcfunction(script_wt *wt, script_func func, void *ctx)
{
    lua_pushlightuserdata(wt->L, func);
    lua_pushlightuserdata(wt->L, ctx);
    lua_pushcclosure(wt->L, script_dispatch, 2);
}

int script_do_file(script_t *script, const char *path)
{
    if (luaL_dofile(script->L, path) != LUA_OK)
    {
        LOGGER_ERROR("[Lua Error] %s", lua_tostring(script->L, -1));
        lua_pop(script->L, 1); // limpa stack
        return 0;
    }

    return 1;
}

int script_get_top(script_wt *wt)
{
    return lua_gettop(wt->L);
}

void script_get_global(script_wt *wt, const char *name)
{
    lua_getglobal(wt->L, name);
}

int script_loadfile(script_wt *wt, const char *path)
{
    return luaL_loadfile(wt->L, path);
}

void script_pushvalue(script_wt *wt, int idx)
{
    lua_State *L = wt->L;
    lua_pushvalue(L, idx);
}

const char *script_setupvalue(script_wt *wt, int func_index, int n)
{
    lua_State *L = wt->L;
    return lua_setupvalue(L, func_index, n);
}

int script_pcall(script_wt *wt, int nargs, int nresults)
{
    lua_State *L = wt->L;
    return lua_pcall(L, nargs, nresults, 0);
}

int script_ref(script_wt *wt)
{
    lua_State *L = wt->L;
    return luaL_ref(L, LUA_REGISTRYINDEX);
}

void script_unref(script_wt *wt, int ref)
{
    lua_State *L = wt->L;
    luaL_unref(L, LUA_REGISTRYINDEX, ref);
}

void script_rawgetref(script_wt *wt, int ref)
{
    lua_State *L = wt->L;
    lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
}

int script_isfunction(script_wt *wt, int index)
{
    return lua_isfunction(wt->L, index);
}

void script_set_global(script_wt *wt, const char *name)
{
    lua_setglobal(wt->L, name);
}

// INTERNAL FUNCTIONS

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
        .ctx = ctx};

    return func(&wt);
}

static int script_traceback(lua_State *L)
{
    const char *msg = lua_tostring(L, 1);
    if (msg)
        luaL_traceback(L, L, msg, 1);
    else
        lua_pushliteral(L, "(erro desconhecido)");

    return 1;
}
