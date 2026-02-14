#include "engine.h"

#include <unistd.h>
#include <string.h>

extern "C" {
    void print(const char *str) {
        write(1, str, strlen(str));
        write(1, "\n", 1);
    }
}