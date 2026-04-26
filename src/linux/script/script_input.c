#include <monny/script/script.h>
#include <monny/input/input.h>
#include <monny/core/engine.h>

static int l_input_key_down(script_wt* wt) {
    input_t *input = script_get_context(wt);

    const char *action = script_checkstring(wt, 1);

    int down = input_key_down(input, action);

    script_pushboolean(wt, down);
    return 1;
}

static int l_input_key_released(script_wt* wt) {
    input_t *input = script_get_context(wt);

    const char *action = script_checkstring(wt, 1);

    int down = input_key_released(input, action);

    script_pushboolean(wt, down);
    return 1;
}

static int l_input_key_pressed(script_wt* wt) {
    input_t *input = script_get_context(wt);

    const char *action = script_checkstring(wt, 1);

    int down = input_key_pressed(input, action);

    script_pushboolean(wt, down);
    return 1;
}

void input_script_register(script_t *script, input_t *input)
{
    script_register_function(
        script,
        "input",          // namespace
        "is_down",        // função
        l_input_key_down,
        input             // contexto
    );
    script_register_function(
        script,
        "input",          // namespace
        "is_released",        // função
        l_input_key_released,
        input             // contexto
    );
    script_register_function(
        script,
        "input",          // namespace
        "is_pressed",        // função
        l_input_key_pressed,
        input             // contexto
    );
}

static void input_module_resgiter(script_t *script, engine_t *engine)
{
    input_script_register(script, engine->input);
}

script_module_t input_module = {
    .name = "input",
    .registry_func = input_module_resgiter};

REGISTER_MODULE(input_module)