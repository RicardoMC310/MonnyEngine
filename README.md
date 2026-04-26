# Monny Engine

Engine de jogo 2D open-source escrita em C, diseñada para ser leve, modular e extensível via scripts Lua.

## Dependências

- [SDL3](https://www.libsdl.org/) — Gerenciamento de janelas e eventos de input
- [Lua](https://www.lua.org/) — Linguagem de script para lógica de jogo
- [cJSON](https://github.com/DaveGamble/cJSON) — Parser JSON para configurações
- [GLAD](https://github.com/DavidLechner/GLAD) — Loader para OpenGL
- [cJSON](https://github.com/DaveGamble/cJSON) — MIT license

---

## Arquitetura Geral

A engine segue uma arquitetura modular com subsistemas independentes acoplados ao núcleo central (`engine_t`). O fluxo principal é:

```
main.c
  └─> engine_init()
        ├─ renderer_init()
        ├─ window_create()
        ├─ input_create()
        ├─ script_create()
        └─ scene_manager_create()

engine_update() loop
  └─> Input → Lua (onUpdate) → Renderer → SwapBuffers
```

```
engine_t
  ├── window_t         (SDL3 + OpenGL context)
  ├── input_t         (keybind + event polling)
  ├── script_t        (Lua state + módulos)
  ├── renderer_t      (comandos + GL)
  └── scene_manager_t (handles + scenes)
```

Cada subsistema é opaco (struct definidas em `.c`, não no header), expondo apenas funções e tipos opacos.

---

## Decisões de Design

### Opaque Types

Todas as structs principais (`window_t`, `renderer_t`, `input_t`, `script_t`, `scene_manager_t`) são definidas apenas nos arquivos `.c` correspondentes. Headers expõem apenas ponteiros opacos (`typedef struct renderer_t renderer_t;`). Isso protege o encapsulamento e permite更改 interna sem quebrar binários.

### Handle System

O `handle_t` é um `uint64_t` que empacota dois campos:
```
bits 0-31   → índice no array
bits 32-63  → generation counter
```

```c
handle_t handle_create(u32 index, u32 generation);
u32 handle_get_index(handle_t h);
u32 handle_get_generation(h);
```

Quando um slot é desalocado, o generation incrementa. O handle antigo expira automaticamente — verificação de validade é O(1). Isso é usado no Scene Manager para referências a cenas sem ponteiros diretos que podem se tornar dangling.

### Modular Script System (Auto-registro via ELF Sections)

Módulos Lua são registrados automaticamente via a macro `REGISTER_MODULE`:

```c
#define REGISTER_MODULE(module)                       \
    static script_module_t *__module_##module       \
        __attribute__((used, section("modules"))) = &module;
```

O linker coleta todos os módulos entre os símbolos `__start_modules` e `__stop_modules` (ELF sections), chamando `registry_func` para cada um:

```c
for (script_module_t **mod = &__start_modules; mod < &__stop_modules; mod++) {
    (*mod)->registry_func(script, ctx);
}
```

Isso elimina necessidade de registro manual — cada `.c` que define um módulo é automaticamente integrado.

### Lua Binding (lightuserdata + upvalues)

Funções C expostas ao Lua são armazenadas como `lightuserdata` nos upvalues:

```c
static int script_dispatch(lua_State *L) {
    script_func func = lua_touserdata(L, lua_upvalueindex(1));
    void *ctx = lua_touserdata(L, lua_upvalueindex(2));
    script_wt wt = { .L = L, .ctx = ctx };
    return func(&wt);
}
```

- **Vantagem**: zero alocação heap para bindings — lightuserdata é apenas um ponteiro.
- **Contexto por módulo**: cada função recebe seu próprio contexto (ex: `engine*` para `input_*`).

### Type-Safe Call Interface (sig, va_args)

Chamadas Lua→C usam um sistema de assinatura de tipos por string:

```c
script_call_ref_function(script, ref, "dd>d", 2.0, 3.0, &result);
//  "dd>d" → double, double, returns> double
```

O parser de assinatura itera os caracteres e empilha valores via `va_arg`, permitindo chamadas type-safe sem lua_call direto.

### Renderer Command Buffer

O renderer mantém um buffer circular (inicia em 1024, dobra em overflow):

```c
struct renderer_t {
    renderer_command_t *g_commands;
    usize g_count;
    usize g_capacity;
};
```

Fluxo por frame:
```
renderer_begin()    → zera count
scene_render()      → renderer_submit() empilha comando
renderer_end()      → itera comandos, glClear + SDL_GL_SwapWindow
```

### Input: Keybind Hash-Based Lookup

Keybinds são mapeados por nome (string) para scancode, com verificação dupla (hash + strcmp):

```c
input_key_down(input, "move_left")
  1. hash_string("move_left")
  2. busca linear por hash match + strcmp
```

O estado de input tem 3 arrays para os 3 estados:
- `keys[]` — estado atual
- `keys_pressed[]` — frame que foi pressionado
- `keys_released[]` — frame que foi solto

### Scene Manager

Cenas são armazenadas em array com alocação linear. Handles previnem acesso a cenas destruídas:

```c
handle_t scene_manager_new_scene(sm, "main");
scene_manager_swap_current_scene(sm, handle);
// handle expira se a cena for removida (generation mismatch)
```

Cada scene define cor de fundo RGBA (normalizada 0.0–1.0 para GL).

### Config via JSON (cJSON)

Configurações são carregadas de `config.json` usando parser genérico com field descriptors:

```c
config_field_t fields[] = {
    {"window", offsetof(engine_config_t, window), TYPE_OBJECT, 0, parse_window},
    {"target_fps", offsetof(engine_config_t, target_fps), TYPE_INT, 0, NULL},
    {"keybinds", offsetof(...), TYPE_ARRAY, 0, parse_keybinds},
    ...
};
```

O parser genérico usa `offsetof` para escrever diretamente nos campos da struct via ponteiro. Permite adicionar novos campos declarando um entry no array, sem modificar o parser.

### Tipos Fixos

Para portability e clareza, typedefs fixos:

```c
typedef uint8_t  u8;   typedef int8_t  i8;
typedef uint16_t u16;  typedef int16_t i16;
typedef uint32_t u32;  typedef int32_t i32;
typedef uint64_t u64;  typedef int64_t i64;
typedef float    f32;
typedef double  f64;
```

### Multi-platform via CMake

CMake detecta SO e seleciona fonte:

```cmake
if(WIN32)
    set(PLATFORM win32)
elseif(UNIX)
    set(PLATFORM linux)
endif()
file(GLOB_RECURSE SOURCES "src/${PLATFORM}/*.c")
```

Cada plataforma pode ter implementações específicas (ex: `src/linux/window.c` vs `src/win32/main.c`).

---

## Estrutura de Diretórios

```
Engine/
├── include/monny/          # Headers públicos
│   ├── core/                # engine.h, config.h, handle.h, logger.h, assets_config.h
│   ├── renderer/            # renderer.h, renderer_backend.h
│   ├── scene/               # scene.h, scene_manager.h
│   ├── script/              # script.h
│   ├── input/               # input.h
│   ├── window/              # window.h
│   ├── helpers/             # hash_string.h
│   └── types.h
├── src/
│   ├── linux/               # Implementação Linux (pode ter src/win32/)
│   │   ├── core/            # engine.c, config.c, handle.c, asset_config.c
│   │   ├── renderer/        # renderer.c, opengl_backend.c
│   │   ├── scene/           # scene.c, scene_manager.c
│   │   ├── script/          # script.c + módulos auto-registrados
│   │   ├── input/           # input.c
│   │   ├── window/          # window.c
│   │   ├── helpers/         # hash_string.c
│   │   └── main.c
│   └── win32/
├── examples/               # main.lua, config.json, assets.json
├── vendor/                  # Dependências (SDL, Lua, cJSON, GLAD)
│   ├── SDL/
│   ├── SDL_image/
│   ├── lua/
│   ├── cJSON/
│   └── glad/
├── external/                # Wrappers Lua customizados
├── CMakeLists.txt
└── README.md
```

---

## Ciclo de Vida do Jogo

```lua
-- main.lua
function onSetup()
    -- Chamado uma vez após engine_init()
end

function onUpdate(deltaTime)
    -- Chamado a cada frame, recebe deltaTime em segundos
end

function onStop()
    -- Chamado antes do engine shutdown
end
```

O deltaTime é calculado via `SDL_GetPerformanceCounter`:

```c
Uint64 current = SDL_GetPerformanceCounter();
f64 deltaTime = (current - last) / SDL_GetPerformanceFrequency();
```

FPS counter é atualizado a cada segundo (`fps_accum >= 1.0`).

---

## Módulos Lua Disponíveis

| Módulo | Funções |
|--------|---------|
| `input` | `is_down(action)`, `is_pressed(action)`, `is_released(action)` |
| `engine` | `get_fps()` |
| `scene` | `new_scene(name)`, `set_current_scene(scene)`, `get_current_scene()` |
| `Scene` (metatable) | `set_background(r, g, b)`, `get_name()` |

---

## Exemplo de Uso

```json
// config.json
{
    "version": "1.0.0",
    "file_main": "main.lua",
    "window": { "title": "Meu Jogo", "width": 640, "height": 480 },
    "target_fps": 60,
    "keybinds": [
        {"action": "move_left", "key": "A"},
        {"action": "jump",      "key": "Space"}
    ]
}
```

```lua
-- main.lua
local current_scene = nil

function onSetup()
    current_scene = scene.new_scene("game")
    scene.set_current_scene(current_scene)
    current_scene:set_background(30, 30, 80)
    print("FPS: " .. engine.get_fps())
end

function onUpdate(dt)
    if input.is_down("move_left") then
        print("Mover esquerda")
    end
end
```

---

## Build

```bash
cmake -B build && cmake --build build
./build/mengine
```

O executável espera `config.json` e `main.lua` no diretório de execução.

---

## Limitações e Notas

- **OpenGL 3.3 Core Profile** — escolha por compatibilidade e tooling
- **Double Buffer** habilitado (para VSync ou frame pacing)
- **Sem ECS ainda** — cenas armazenam apenas cor de background
- **Renderer é immediate-mode** — comandos são processados em `renderer_end()`
- **Lua embeds modules via lightuserdata** — não há garbage collection de ponteiros C

---

## Roadmap Informal

- [ ] Renderer 2D (sprites, texturas via SDL_image)
- [ ] Entity/Component base
- [ ] Audio (SDL_mixer)
- [ ] Tilemap support
- [ ] Physics básico (AABB collision)
- [ ] Editor de cenas (imGui?)