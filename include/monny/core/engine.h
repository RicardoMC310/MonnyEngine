#ifndef __ENGINE_ENGINE_H__
#define __ENGINE_ENGINE_H__

#include "monny/core/config.h"

typedef struct engine_t engine_t;

engine_t *engine_init(engine_config_t *config);
void engine_stop(engine_t *engine);

#endif
