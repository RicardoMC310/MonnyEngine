#ifndef __ENGINE_WINDOW_H__
#define __ENGINE_WINDOW_H__

#include "monny/core/config.h"
#include "monny/types.h"

typedef struct window_t window_t;

window_t *window_create(window_config_t *window_config);
void window_destroy(window_t *window);

#endif
