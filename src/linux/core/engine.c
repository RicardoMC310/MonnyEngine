#include <monny/core/engine.h>
#include <monny/core/logger.h>
#include <monny/window/window.h>
#include <monny/input/input.h>
#include <monny/script/script.h>
#include <monny/renderer/renderer.h>
#include <monny/scene/scene.h>
#include <monny/scene/scene_manager.h>

#include <SDL3/SDL.h>

#include <stdlib.h>

struct engine_t
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
};

engine_t *engine_init(engine_config_t *config)
{
	engine_t *app = malloc(sizeof(engine_t));
	if (!app)
	{
		LOGGER_ERROR("Erro ao inicializar engine");
		return NULL;
	}

	app->config = config;

	app->renderer = renderer_init();
	app->window = window_create(&app->config->window);

	app->input = input_create(app->config->keybinds, app->config->keybind_count);

	app->script = script_create(app->config->file_main);

	app->scene_manager = scene_manager_create();

	renderer_create_context(app->renderer, app->window);

	input_script_register(app->script, app->input);
	engine_script_register(app->script, app);

	scene_manager_script_register(app->script, app->scene_manager);

	app->ref_func_script_onSetup = script_load_ref_function(app->script, "onSetup");
	app->ref_func_script_onUpdate = script_load_ref_function(app->script, "onUpdate");
	app->ref_func_script_onStop= script_load_ref_function(app->script, "onStop");

	script_call_ref_function(app->script, app->ref_func_script_onSetup, "");

	return app;
}

void engine_stop(engine_t *app)
{
	if (!app)
		return;

	script_call_ref_function(app->script, app->ref_func_script_onStop, "");

	if (app->window)
		window_destroy(app->window);

	if (app->input)
		input_destroy(app->input);

	if (app->script)
		script_destroy(app->script);

	if (app->renderer)
		renderer_destroy(app->renderer);

	if (app->scene_manager)
		scene_manager_destroy(app->scene_manager);

	free(app);
	app = NULL;
}

void engine_update(engine_t *app)
{
	if (!app)
		return;

	Uint64 freq = SDL_GetPerformanceFrequency();
	Uint64 last = SDL_GetPerformanceCounter();

	handle_t main_scene = scene_manager_new_scene(app->scene_manager, "mainscene");
	scene_manager_swap_current_scene(app->scene_manager, main_scene);

	app->fps = 0.0;

	while (!input_should_quit(app->input))
	{
		Uint64 current = SDL_GetPerformanceCounter();

		f64 deltaTime = (f64)(current - last) / (f64)freq;
		last = current;

		if (deltaTime <= 0.000001)
			deltaTime = 0.000001; // anti-inf

		app->fps_accum += deltaTime;
		app->fps_count++;

		if (app->fps_accum >= 1.0)
		{
			app->fps = app->fps_count;
			app->fps_count = 0;
			app->fps_accum = 0;
		}
		input_listener_event(app->input);
		script_call_ref_function(app->script, app->ref_func_script_onUpdate, "d", deltaTime);

		renderer_begin(app->renderer);
		scene_t *current_scene = scene_manager_get_current_scene(app->scene_manager);
		scene_render(current_scene, app->renderer);
		renderer_end(app->renderer);

		Uint64 frame_time = SDL_GetPerformanceCounter() - current;
		double frame_sec = (double)frame_time / freq;

		double target = 1.0 / app->config->target_fps;

		if (frame_sec < target)
		{
			SDL_Delay((Uint32)((target - frame_sec) * 1000.0));
		}
	}
}

f64 engine_get_fps(engine_t *engine)
{
	if (!engine)
		return 0.0;

	return engine->fps;
}