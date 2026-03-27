#include <monny/core/engine.h>
#include <monny/core/logger.h>
#include <monny/window/window.h>
#include <monny/input/input.h>

#include <SDL3/SDL.h>

#include <stdlib.h>

struct engine_t
{
	engine_config_t *config;
	window_t *window;
	input_t *input;
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

	free(app);
	app = NULL;
}

void engine_update(engine_t *app)
{
	if (!app)
		return;

	while(!input_should_quit(app->input)) {

		input_listener_event(app->input);

		if (input_key_down(app->input, "move_left")) {
			LOGGER_INFO("Andando Para Esquerda");
		} else if (input_key_down(app->input, "move_right")) {
			LOGGER_INFO("Andando Para Direita");
		}

		if (input_key_down(app->input, "move_up")) {
			LOGGER_INFO("Andando Para Cima");
		} else if (input_key_down(app->input, "move_down")) {
			LOGGER_INFO("Andando Para Baixo");
		}

		if (input_key_pressed(app->input, "jump")) {
			LOGGER_INFO("Pulou");
		}

		if (input_key_released(app->input, "attack")) {
			LOGGER_INFO("Atacou");
		}

		SDL_Delay(1000 / app->config->target_fps);
	}
}