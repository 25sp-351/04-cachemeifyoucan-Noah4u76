#include "cache.h"
#include <stdlib.h>
#include <stdio.h>

struct Cache {
    int capacity;
    int size;
    CacheEntry entries[CACHE_CAPACITY];
};

static int find_index_recursive(Cache *cache, int rod_length, int startIndex);
static void move_to_front(Cache *cache, int index);
static void shift_right(CacheEntry entries[], int fromIndex, int toIndex);
static void free_entry(CacheEntry *entry);


Cache* cache_create()
{
    Cache *cache = (Cache*)malloc(sizeof(Cache));
    if (!cache) return NULL;

    cache->capacity = CACHE_CAPACITY;
    cache->size = 0;

    for (int ix = 0; ix < CACHE_CAPACITY; ix++) {
        cache->entries[ix].rod_length = -1;
        cache->entries[ix].total_value = 0;
        cache->entries[ix].cuts = NULL;
    }
    return cache;
}

void cache_destroy(Cache* cache)
{
    if (!cache) return;
    // Free all cuts
    for (int ix = 0; ix < cache->size; ix++) {
        free_entry(&cache->entries[ix]);
    }
    free(cache);
}

CacheEntry* cache_get(Cache* cache, int rod_length)
{
    // Recursively search for rod_length
    int index = find_index_recursive(cache, rod_length, 0);
    if (index < 0) {
        return NULL;
    }

    // If found, move that entry to the front (index 0) => MRU
    move_to_front(cache, index);

    return &cache->entries[0];
}

void cache_put(Cache* cache, int rod_length, int *cuts, int total_value)
{
    // Check if it already exists
    int index = find_index_recursive(cache, rod_length, 0);
    if (index >= 0) {
        // Already in cache. Update it, then move to front.
        free_entry(&cache->entries[index]); // free old cuts
        cache->entries[index].rod_length  = rod_length;
        cache->entries[index].total_value = total_value;
        cache->entries[index].cuts        = cuts;

        move_to_front(cache, index);
        return;
    }

    // Not found in cache
    // If not full, we can just insert a new entry
    if (cache->size < cache->capacity) {
        // SHIFT everything from 0..size-1 to the right by 1
        shift_right(cache->entries, 0, cache->size);
        // Place new item at index 0
        cache->entries[0].rod_length  = rod_length;
        cache->entries[0].total_value = total_value;
        cache->entries[0].cuts        = cuts;

        cache->size++;
    }
    else {
        // The cache is FULL, so evict LRU (the last entry)
        free_entry(&cache->entries[cache->size - 1]);

        // SHIFT everything from 0..(size-2) to the right by 1
        shift_right(cache->entries, 0, cache->size - 1);

        // Place new item at index 0
        cache->entries[0].rod_length  = rod_length;
        cache->entries[0].total_value = total_value;
        cache->entries[0].cuts        = cuts;
        // size remains the same
    }
}

/**
 * Recursively searches for rod_length in entries[startIndex..(size-1)].
 * Returns the index if found, or -1 if not found.
 */
static int find_index_recursive(Cache *cache, int rod_length, int startIndex)
{
    if (startIndex >= cache->size) {
        return -1;
    }
    if (cache->entries[startIndex].rod_length == rod_length) {
        return startIndex;
    }
    // Recurse to the next index
    return find_index_recursive(cache, rod_length, startIndex + 1);
}

/**
 * Move the entry at 'index' to the front (index 0) by shifting.
 * In an LRU scheme, index 0 is the MRU.
*/
static void move_to_front(Cache *cache, int index)
{
    if (index <= 0) {
        // already at front or invalid
        return;
    }
    CacheEntry temp = cache->entries[index];

    // shift everything from 0..(index-1) right by 1. We'll do it recursively:
    // shift_right(array, start, end) shifts the block [start..end] to [start+1..end+1].
    shift_right(cache->entries, 0, index - 1);

    cache->entries[0] = temp;
}

/**
 * shift_right(entries, fromIndex, toIndex) recursively shifts
 * the array segment [fromIndex..toIndex] one position to the *right*,
*/
static void shift_right(CacheEntry entries[], int fromIndex, int toIndex)
{
    if (toIndex < fromIndex) {
        return;
    }
    entries[toIndex + 1] = entries[toIndex];

    shift_right(entries, fromIndex, toIndex - 1);
}

/**
 * Free the dynamically allocated 'cuts' array, if any.
 */
static void free_entry(CacheEntry *entry)
{
    if (entry->cuts) {
        free(entry->cuts);
        entry->cuts = NULL;
    }
    entry->rod_length  = -1;
    entry->total_value = 0;
}
