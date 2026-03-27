#include "monny/core/config.h"
#include "monny/core/logger.h"
#include "monny/core/engine.h"

int main(void)
{
	engine_config_t config = config_load_defaults();
    
	config_load_file(&config);
	engine_t *app = engine_init(&config);

	engine_update(app);

	engine_stop(app);
	config_clear(&config);

	return 0;
}
