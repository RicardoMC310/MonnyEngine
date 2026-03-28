#ifndef __ENGINE_ENGINE_H__
#define __ENGINE_ENGINE_H__

#include <monny/core/config.h>
#include <monny/script/script.h>

typedef struct engine_t engine_t;

engine_t *engine_init(engine_config_t *config);
void engine_stop(engine_t *app);
void engine_update(engine_t *app);
f64 engine_get_fps(engine_t *engine);

void engine_script_register(script_t *script, engine_t *engine);

#endif
