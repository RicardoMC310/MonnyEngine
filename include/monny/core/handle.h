#ifndef __ENGINE_HANDLE_H__
#define __ENGINE_HANDLE_H__

#include <monny/types.h>

typedef u64 handle_t;

handle_t handle_create(u32 index, u32 generation);
u32 handle_get_index(handle_t h);
u32 handle_get_generation(handle_t h);

#endif