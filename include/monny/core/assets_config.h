#ifndef __ENGINE_ASSETS_CONFIG_H__
#define __ENGINE_ASSETS_CONFIG_H__

#include <monny/types.h>

#define MAX_CHARACTERS_IN_ASSET_NAME 64
#define MAX_CHARACTERS_IN_ASSET_TYPE 64
#define MAX_CHARACTERS_IN_ASSET_PATH 128

typedef struct asset_t {
    char name[MAX_CHARACTERS_IN_ASSET_NAME];
    char type[MAX_CHARACTERS_IN_ASSET_TYPE];
    char path[MAX_CHARACTERS_IN_ASSET_PATH];
} asset_t;

typedef struct asset_config_t {
    asset_t *assets;
    usize assets_count;
    usize assets_capacity;
} asset_config_t;

asset_config_t assets_config_load_defaults();
void assets_config_load_file(asset_config_t *out, const char *path);
void assets_config_clear(asset_config_t *config);

#endif