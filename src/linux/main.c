#include "monny/core/config.h"
#include "monny/core/logger.h"
#include "monny/core/engine.h"

int main(void)
{
	engine_config_t config = config_load_defaults();
    
	config_load_file(&config);

	assets_config_load_file(&config.asset_config, config.asset_file);

	engine_t *app = engine_init(&config);

	engine_update(app);
	
	engine_stop(app);
	assets_config_clear(&config.asset_config);
	config_clear(&config);

	return 0;
}
