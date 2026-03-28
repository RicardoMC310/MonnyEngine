#ifndef __ENGINE_SCRIPT_H__
#define __ENGINE_SCRIPT_H__

typedef struct script_wt script_wt;
typedef struct script_t script_t;

typedef int (*script_func)(script_wt *wt);

script_t *script_create(const char *filepath);
void script_destroy(script_t *script);
void script_update(script_t *script, double deltaTime);
void script_register_function(
    script_t *script,
    const char *table,
    const char *name,
    script_func func,
    void *ctx);

const char *script_checkstring(script_wt *wt, int index);
int script_checkint(script_wt *wt, int index);
void script_pushboolean(script_wt *wt, int value);
void script_pushnumber(script_wt *wt, double value);
void *script_get_context(script_wt *wt);

#endif