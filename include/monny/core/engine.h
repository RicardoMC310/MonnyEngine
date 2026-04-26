#ifndef __ENGINE_ENGINE_H__
#define __ENGINE_ENGINE_H__

#include <monny/core/config.h>
#include <monny/script/script.h>
#include <monny/window/window.h>
#include <monny/input/input.h>
#include <monny/script/script.h>
#include <monny/renderer/renderer.h>
#include <monny/scene/scene_manager.h>

typedef struct engine_t
{
    engine_config_t *config;
    window_t *window;
    input_t *input;
    script_t *script;
    renderer_t *renderer;
    scene_manager_t *scene_manager;

    u32 ref_func_script_onSetup;
    u32 ref_func_script_onUpdate;
    u32 ref_func_script_onStop;

    f64 fps;
    f64 fps_accum;
    f64 fps_count;
} engine_t;

engine_t *engine_init(engine_config_t *config);
void engine_stop(engine_t *app);
void engine_update(engine_t *app);
f64 engine_get_fps(engine_t *engine);

#endif
