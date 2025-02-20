#ifndef CACHE_H
#define CACHE_H

#include "pieces.h"

#define CACHE_CAPACITY 10

typedef struct {
    int rod_length;
    int total_value;
    int *cuts; // dynamically allocated array for cuts
} CacheEntry;

typedef struct Cache Cache;

Cache* cache_create();

void cache_destroy(Cache* cache);

CacheEntry* cache_get(Cache* cache, int rod_length);

void cache_put(Cache* cache, int rod_length, int *cuts, int total_value);

#endif
