#include <monny/core/assets_config.h>
#include <monny/core/logger.h>

#include "cJSON.h"

#include <stdio.h>
#include <stdlib.h>

static void parse_config(char *buffer, asset_config_t *out, const char *path);

asset_config_t assets_config_load_defaults()
{
    return (asset_config_t) {
        .assets = NULL,
        .assets_capacity = 0,
        .assets_count = 0
    };
}

void assets_config_load_file(asset_config_t *out, const char *path)
{
    out->assets_capacity = 8;
    out->assets_count = 0;
    out->assets = calloc(out->assets_capacity, sizeof(asset_t));

    FILE *file = fopen(path, "rb");
    if (!file)
    {
        LOGGER_WARN("Arquivo %s encontra-se ausente; Carregando configurações padrões.", path);
        return;
    }

    fseek(file, 0, SEEK_END); 
    size_t size = ftell(file);
    if (size <= 0)
    {
        fclose(file);
        LOGGER_ERROR("Arquivo %s está vazio", path);
        return;
    }

    rewind(file);

    char *buffer = malloc(size + 1);
    if (!buffer)
    {
        fclose(file);
        LOGGER_ERROR("Erro ao criar buffer de dados para conter o arquivo %s", path);
        return;
    }

    fread(buffer, 1, (size_t)size, file);
    buffer[size] = '\0';
    fclose(file);

    parse_config(buffer, out, path);

    free(buffer);
}

void assets_config_clear(asset_config_t *config)
{
    if (!config) return;

    if (config->assets)
    {
        free(config->assets);
        config->assets = NULL;
    }

    config->assets_capacity = config->assets_count = 0;
}

static void parse_config(char *buffer, asset_config_t *out, const char *path)
{
    cJSON *root = cJSON_Parse(buffer);
    if (!root)
    {
        LOGGER_ERROR("Erro ao fazer parse do JSON no %s", path);
        return;
    }

    cJSON *assetsArrayOBJ = cJSON_GetObjectItem(root, "assets");

    if (!cJSON_IsArray(assetsArrayOBJ))
    {
        LOGGER_ERROR("Erro ao fazer parse do JSON no %s, assets não é um array.", path);
        return;
    }

    usize i = 0;
    cJSON *asset = NULL;

    cJSON_ArrayForEach(asset, assetsArrayOBJ)
    {
        if (i >= out->assets_capacity)
        {
            out->assets_capacity += 8;
            out->assets = realloc(out->assets, sizeof(asset_t) * out->assets_capacity);
        }

        cJSON *name = cJSON_GetObjectItem(asset, "name");
        cJSON *type = cJSON_GetObjectItem(asset, "type");
        cJSON *path = cJSON_GetObjectItem(asset, "path");

        if (cJSON_IsString(name)) 
            strncpy(out->assets[i].name, name->valuestring, sizeof(out->assets[i].name) - 1);

        if (cJSON_IsString(type))
            strncpy(out->assets[i].type, type->valuestring, sizeof(out->assets[i].type) - 1);

        if (cJSON_IsString(path))
            strncpy(out->assets[i].path, path->valuestring, sizeof(out->assets[i].path) - 1);

        i++;
    }

    out->assets_count = i;

    cJSON_Delete(root);
}