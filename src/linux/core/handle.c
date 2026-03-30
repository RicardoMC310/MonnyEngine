#include <monny/core/handle.h>

#define HANDLE_INDEX_MASK 0xFFFFFFFF
#define HANDLE_GENERATION_SHIFT 32

handle_t handle_create(u32 index, u32 generation)
{
    return ((u64)generation << HANDLE_GENERATION_SHIFT) | index;
}

u32 handle_get_index(handle_t h)
{
    return (u32)(h & HANDLE_INDEX_MASK);
}

u32 handle_get_generation(handle_t h)
{
    return (u32)(h >> HANDLE_GENERATION_SHIFT);
}