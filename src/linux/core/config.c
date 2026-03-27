#include "monny/core/config.h"
#include "monny/core/logger.h"

#include "cJSON.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum
{
    TYPE_INT,
    TYPE_OBJECT,
    TYPE_ARRAY,
    TYPE_DOUBLE,
    TYPE_STRING
} field_type_t;

typedef struct
{
    const char *json_key;
    size_t offset;
    field_type_t type;
    size_t max_len;
    void (*parse_fn)(cJSON *item, void *ptr);
} config_field_t;

void parse_config(char *data, engine_config_t *out);
void parse_window(cJSON *screen_object, void *out);
void parse_keybinds(cJSON *keybinds_object, void *out);

config_field_t fields[] = {
    {"window", offsetof(engine_config_t, window), TYPE_OBJECT, 0, parse_window},
    {"target_fps", offsetof(engine_config_t, target_fps), TYPE_INT, 0, NULL},
    {"keybinds", offsetof(engine_config_t, keybinds), TYPE_ARRAY, 0, parse_keybinds},
    {"version", offsetof(engine_config_t, version), TYPE_STRING, sizeof(((engine_config_t *)0)->version), NULL},
    {"file_main", offsetof(engine_config_t, file_main), TYPE_STRING, sizeof(((engine_config_t *)0)->file_main), NULL}};
size_t num_fields = sizeof(fields) / sizeof(fields[0]);

engine_config_t config_load_defaults()
{
    return (engine_config_t){
        .window = {
            .title = "Default Title",
            .width = 800,
            .height = 600},
        .target_fps = 60,
        .keybind_count = 0,
        .keybind_size = 0,
        .keybinds = NULL,
        .version = "1.0.0",
        .file_main = "main.lua"};
}

void config_load_file(engine_config_t *out)
{
    out->keybind_size = 8;
    out->keybind_count = 0;
    out->keybinds = malloc(sizeof(keybind_config_t) * out->keybind_size);

    FILE *config_file = fopen("config.json", "rb");

    if (!config_file)
    {
        LOGGER_ERROR("Arquivo config.json encontra-se ausente");
        return;
    }

    if (fseek(config_file, 0, SEEK_END) != 0)
    {
        fclose(config_file);
        return;
    }

    size_t size = ftell(config_file);
    if (size <= 0)
    {
        fclose(config_file);
        LOGGER_ERROR("Arquivo config.json esta vazio");
        return;
    }

    rewind(config_file);

    char *buffer = malloc(size + 1);
    if (!buffer)
    {
        fclose(config_file);
        LOGGER_ERROR("Erro ao criar buffer de dados para conter o arquivo config.json");
        return;
    }

    fread(buffer, 1, (size_t)size, config_file);
    buffer[size] = '\0';
    fclose(config_file);

    parse_config(buffer, out);

    free(buffer);
}

void config_clear(engine_config_t *config)
{
    if (!config)
        return;

    if (config->keybinds)
        free(config->keybinds);

    config->keybind_count = config->keybind_size = 0;
}

void parse_window(cJSON *screen_object, void *ptr)
{
    engine_config_t *out = (engine_config_t *)ptr;
    cJSON *item;

    item = cJSON_GetObjectItem(screen_object, "title");
    if (cJSON_IsString(item))
        strncpy(out->window.title, item->valuestring, sizeof(out->window.title) - 1);

    item = cJSON_GetObjectItem(screen_object, "width");
    if (cJSON_IsNumber(item))
        out->window.width = item->valueint;

    item = cJSON_GetObjectItem(screen_object, "height");
    if (cJSON_IsNumber(item))
        out->window.height = item->valueint;
}

void parse_keybinds(cJSON *keybinds_object, void *ptr)
{
    engine_config_t *out = (engine_config_t *)ptr;

    size_t i = 0;
    cJSON *kb = NULL;
    cJSON_ArrayForEach(kb, keybinds_object)
    {
        if (out->keybind_count >= out->keybind_size)
        {
            out->keybind_size += 8;
            out->keybinds = realloc(out->keybinds, sizeof(keybind_config_t) * out->keybind_size);
        }

        cJSON *action = cJSON_GetObjectItem(kb, "action");
        cJSON *key = cJSON_GetObjectItem(kb, "key");

        if (cJSON_IsString(action))
            strncpy(out->keybinds[i].action, action->valuestring, sizeof(out->keybinds[i].action) - 1);

        if (cJSON_IsString(key))
            strncpy(out->keybinds[i].key, key->valuestring, sizeof(out->keybinds[i].key) - 1);

        i++;
    }
    out->keybind_count = i;
}

void parse_config(char *data, engine_config_t *out)
{
    if (!data)
    {
        LOGGER_ERROR("Buffer de dados encontra-se vazio");
        return;
    }

    cJSON *root = cJSON_Parse(data);

    if (!root)
    {
        LOGGER_ERROR("Erro ao fazer parse do JSON no config.json");
        return;
    }

    size_t i;
    for (i = 0; i < num_fields; i++)
    {
        cJSON *item = cJSON_GetObjectItem(root, fields[i].json_key);
        if (!item)
            continue;

        void *field_ptr = (char *)out + fields[i].offset;

        switch (fields[i].type)
        {
        case TYPE_INT:
        {
            if (cJSON_IsNumber(item))
                *(int *)field_ptr = item->valueint;
            break;
        }

        case TYPE_DOUBLE:
        {
            if (cJSON_IsNumber(item))
                *(double *)field_ptr = item->valuedouble;
            break;
        }

        case TYPE_STRING:
        {
            if (cJSON_IsString(item))
                strncpy((char *)field_ptr, item->valuestring, fields[i].max_len - 1);
            break;
        }

        case TYPE_OBJECT:
        {
            if (cJSON_IsObject(item))
                if (fields[i].parse_fn)
                    fields[i].parse_fn(item, out);
            break;
        }

        case TYPE_ARRAY:
        {
            if (cJSON_IsArray(item))
                if (fields[i].parse_fn)
                    fields[i].parse_fn(item, out);
            break;
        }

        default:
            continue;
        }
    }

    cJSON_Delete(root);
}
