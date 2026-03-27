#include <monny/helpers/hash_string.h>
#include <ctype.h>

u32 hash_string(const char *str)
{
    u32 hash = 2166136261u;

    while (*str)
    {
        char c = tolower(*str);
        hash ^= (u32)(unsigned char)c;
        hash *= 16777619u;
        str++;
    }

    return hash;
}