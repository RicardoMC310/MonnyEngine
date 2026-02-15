#include "monny/core/config.h"
#include "monny/core/logger.h"

int main(void)
{
    engine_config_t config = {
        .window = {
            .title = "Default Title",
            .width = 800,
            .height = 640
        },
        .target_fps = 60,
        .keybind_count = 0,
        .keybind_size = 0,
        .keybinds = NULL
    };

    config_load(&config);

    LOGGER_INFO("Config.Title = %s", config.window.title);
    LOGGER_INFO("Config.Screen_Width = %d", config.window.width);
    LOGGER_INFO("Config.Screen_Height = %d", config.window.height);
    LOGGER_INFO("Config.Target_FPS = %d", config.target_fps);
    LOGGER_INFO("Config.KeyBinds: ");
    int i;
    for (i = 0; i < config.keybind_count; i++)
    {
        keybind_t keybind = config.keybinds[i];

        LOGGER_INFO("action = %s | key = %s", keybind.action, keybind.key);
    }

    return 0;
}