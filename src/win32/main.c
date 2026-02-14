#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
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