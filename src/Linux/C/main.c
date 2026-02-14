#include "engine.h"

int main(void)
{
    #ifdef D_WIN32
        print("Windows");
    #elif defined(D_LINUX)
        print("Linux");
    #else
        print("Sistema desconhecido");
    #endif

    return 0;
}