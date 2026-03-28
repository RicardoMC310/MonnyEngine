#include <monny/core/engine.h>
#include <monny/core/logger.h>
#include <monny/window/window.h>
#include <monny/input/input.h>
#include <monny/script/script.h>

#include <SDL3/SDL.h>

#include <stdlib.h>

struct engine_t
{
	engine_config_t *config;
	window_t *window;
	input_t *input;
	script_t *script;
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
	app->window = window_create(&app->config->window);

	app->input = input_create(app->config->keybinds, app->config->keybind_count);

	app->script = script_create(app->config->file_main);

	input_script_register(app->script, app->input);

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

	free(app);
	app = NULL;
}

void engine_update(engine_t *app)
{
	if (!app)
		return;

	while(!input_should_quit(app->input)) {

		input_listener_event(app->input);

		script_update(app->script, 0);

		SDL_Delay(1000 / app->config->target_fps);
	}
}