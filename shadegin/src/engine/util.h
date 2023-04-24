#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ERROR_EXIT(...)           \
    fprintf(stderr, __VA_ARGS__); \
    exit(1)
#define ERROR_RETURN(R, ...)      \
    fprintf(stderr, __VA_ARGS__); \
    return R

unsigned long long generate_random_id();

#endif
