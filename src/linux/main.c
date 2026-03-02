#include "monny/core/config.h"
#include "monny/core/logger.h"
#include "monny/core/engine.h"

int main(void)
{
	engine_config_t config = {
		.window = {
			.title = "Default Title",
			.width = 800,
			.height = 600},
		.target_fps = 60,
		.keybind_count = 0,
		.keybind_size = 0,
		.keybinds = NULL,
		.version = "1.0.0"};

	config_load(&config);
	engine_t *app = engine_init(&config);

	for (size_t i = 0; i < 1000000000; i++);

	engine_stop(app);
	config_clear(&config);

	return 0;
}
