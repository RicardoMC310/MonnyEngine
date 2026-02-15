#ifndef __ENGINE_LOGGER_H__
#define __ENGINE_LOGGER_H__

#include <stdio.h>
#include <string.h>

#ifdef DEBUG

#define LOGGER_INFO(fmt, ...) \
    do { \
        const char* file = strrchr(__FILE__, '/'); \
        if (!file) file = strrchr(__FILE__, '\\'); \
        file = file ? file + 1 : __FILE__; \
        fprintf(stdout, "[INFO] [%s:%s:%d] " fmt "\n", file, __func__, __LINE__, ##__VA_ARGS__); \
    } while(0)

#define LOGGER_WARN(fmt, ...) \
    do { \
        const char* file = strrchr(__FILE__, '/'); \
        if (!file) file = strrchr(__FILE__, '\\'); \
        file = file ? file + 1 : __FILE__; \
        fprintf(stdout, "[WARN] [%s:%s:%d] " fmt "\n", file, __func__, __LINE__, ##__VA_ARGS__); \
    } while(0)

#define LOGGER_ERROR(fmt, ...) \
    do { \
        const char* file = strrchr(__FILE__, '/'); \
        if (!file) file = strrchr(__FILE__, '\\'); \
        file = file ? file + 1 : __FILE__; \
        fprintf(stderr, "[ERROR] [%s:%s:%d] " fmt "\n", file, __func__, __LINE__, ##__VA_ARGS__); \
    } while(0)

#else

#define LOGGER_INFO(fmt, ...) ((void) 0)

#define LOGGER_WARN(fmt, ...) ((void) 0)

#define LOGGER_ERROR(fmt, ...) \
    do { \
        const char* file = strrchr(__FILE__, '/'); \
        if (!file) file = strrchr(__FILE__, '\\'); \
        file = file ? file + 1 : __FILE__; \
        fprintf(stderr, "[ERROR] [%s:%s:%d] " fmt "\n", file, __func__, __LINE__, ##__VA_ARGS__); \
    } while(0)

#endif

#endif