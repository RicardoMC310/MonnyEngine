#include <monny/core/entity.h>

entity_t entity_create()
{
    static u32 next_id = 0;
    return next_id++;
}