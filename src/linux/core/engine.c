#include <monny/core/engine.h>
#include <monny/core/logger.h>
#include <monny/window/window.h>
#include <monny/input/input.h>
#include <monny/script/script.h>
#include <monny/renderer/renderer.h>
#include <monny/scene/scene.h>

#include <SDL3/SDL.h>

#include <stdlib.h>

struct engine_t
{
	engine_config_t *config;
	window_t *window;
	input_t *input;
	script_t *script;
	renderer_t *renderer;

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

	renderer_create_context(app->renderer, app->window);

	input_script_register(app->script, app->input);
	engine_script_register(app->script, app);

	return app;
}

void engine_stop(engine_t *app)
{
	if (!app)
		return;

	if (app->window)
		window_destroy(app->window);

	if (app->input)
		input_destroy(app->input);

	if (app->script)
		script_destroy(app->script);

	if (app->renderer)
		renderer_destroy(app->renderer);

	free(app);
	app = NULL;
}

void engine_update(engine_t *app)
{
	if (!app)
		return;

	Uint64 freq = SDL_GetPerformanceFrequency();
	Uint64 last = SDL_GetPerformanceCounter();

	app->fps = 0.0;

	scene_t scene;

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
		script_update(app->script, deltaTime);

		renderer_begin(app->renderer);
		scene_render(&scene, app->renderer);
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