#include <stdio.h>

int main(void)
{

#ifdef D_WIN32
    printf("Windows\n");
#elif defined(D_LINUX)
    printf("Linux\n");
#else
    printf("Desconhecido\n");
#endif

    return 0;
}