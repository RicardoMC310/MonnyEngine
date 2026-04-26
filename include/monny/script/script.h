#ifndef __ENGINE_SCRIPT_H__
#define __ENGINE_SCRIPT_H__

#define SCRIPT_OK 0

typedef struct engine_t engine_t;
typedef struct script_wt script_wt;
typedef struct script_t script_t;

typedef int (*script_func)(script_wt *wt);

typedef struct script_module_t
{
    char name[64];

    void (*registry_func)(script_t *script, engine_t *ctx);
} script_module_t;

#define REGISTER_MODULE(module)               \
    static script_module_t *__module_##module \
        __attribute__((used, section("modules"))) = &module;

script_t *script_create(const char *filepath);
void script_destroy(script_t *script);
void script_init_modules(script_t *script, engine_t *ctx);
void script_register_function(
    script_t *script,
    const char *table,
    const char *name,
    script_func func,
    void *ctx);
void script_register_metatable_function(
    script_t *script,
    const char *metatable,
    const char *name,
    script_func func,
    void *ctx);
void script_call_ref_function(script_t *script, int ref, const char *sig, ...);
int script_load_ref_function(script_t *script, const char *func_name);
const char *script_checkstring(script_wt *wt, int index);
int script_checkint(script_wt *wt, int index);
double script_checknumber(script_wt *wt, int index);
void *script_touserdata(script_wt *wt, int index);
void script_pushboolean(script_wt *wt, int value);
void script_pushnumber(script_wt *wt, double value);
void script_getmetatable(script_wt *wt, const char *name);
void script_setmetatable(script_wt *wt, int index);
void script_pop(script_wt *wt, int n);
void script_pushstring(script_wt *wt, const char *value);
void script_pushnil(script_wt *wt);
const char *script_tostring(script_wt *wt, int index);
void script_pushlightuserdata(script_wt *wt, void *ptr);
void script_pushcfunction(script_wt *wt, script_func func, void *ctx);
void *script_get_context(script_wt *wt);
void script_newtable(script_wt *wt);
void script_setfield(script_wt *wt, int index, const char *key);
void script_getfield(script_wt *wt, int index, const char *key);
int script_do_file(script_t *script, const char *path);
int script_get_top(script_wt *wt);
void script_get_global(script_wt *wt, const char *name);
int script_loadfile(script_wt *wt, const char *path);
void script_pushvalue(script_wt *wt, int idx);
const char *script_setupvalue(script_wt *wt, int func_index, int n);
int script_pcall(script_wt *wt, int nargs, int nresults);
int script_ref(script_wt *wt);
void script_unref(script_wt *wt, int ref);
void script_rawgetref(script_wt *wt, int ref);
int script_isfunction(script_wt *wt, int index);
void script_set_global(script_wt *wt, const char *name);

#endif