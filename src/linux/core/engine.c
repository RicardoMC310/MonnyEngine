#include "monny/core/engine.h"
#include "monny/core/logger.h"
#include "monny/window/window.h"

#include <stdlib.h>

struct engine_t
{
	engine_config_t *config;
	window_t *window;
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

	return app;
}

void engine_stop(engine_t *app)
{
	if (!app)
		return;

	if (app->window)
		window_destroy(app->window);

	free(app);
	app = NULL;
}

void engine_update(engine_t *app)
{
	if (!app)
		return;

	while(!window_should_close(app->window)) {
		window_listener_events(app->window);
	}
}