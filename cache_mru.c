#include "cache.h"
#include <stdlib.h>
#include <stdio.h>

struct Cache {
    int capacity;
    int size;
    CacheEntry entries[CACHE_CAPACITY];
};

/* HELPER DECLARATIONS */
static int find_index_recursive(Cache *cache, int rod_length, int startIndex);
static void shift_left(CacheEntry entries[], int fromIndex, int toIndex);
static void free_entry(CacheEntry *entry);

Cache* cache_create()
{
    Cache* cache = (Cache*)malloc(sizeof(Cache));
    if (!cache) return NULL;
    cache->capacity = CACHE_CAPACITY;
    cache->size = 0;
    for (int i = 0; i < CACHE_CAPACITY; i++) {
        cache->entries[i].rod_length = -1;
        cache->entries[i].cuts = NULL;
        cache->entries[i].total_value = 0;
    }
    return cache;
}

void cache_destroy(Cache* cache)
{
    if (!cache) return;
    for (int i = 0; i < cache->size; i++) {
        free_entry(&cache->entries[i]);
    }
    free(cache);
}

CacheEntry* cache_get(Cache* cache, int rod_length)
{
    int index = find_index_recursive(cache, rod_length, 0);
    if (index < 0) {
        return NULL;
    }
    // MRU policy does not reorder on get (some variants might, but let's keep it simple).
    return &cache->entries[index];
}

void cache_put(Cache* cache, int rod_length, int *cuts, int total_value)
{
    // If the rod_length already exists, update it in place (no reordering).
    int index = find_index_recursive(cache, rod_length, 0);
    if (index >= 0) {
        free_entry(&cache->entries[index]);
        cache->entries[index].rod_length  = rod_length;
        cache->entries[index].total_value = total_value;
        cache->entries[index].cuts        = cuts;
        // We'll treat it as "most recent" => put it at index 0 by shifting
        // Evict the old front if we're truly doing "pure MRU." 
        // For demonstration, let's do: remove the item from index, shift everything left from 0..index-1,
        // then place it at 0. But let's keep it simpler: Just treat "0" as top of stack.

        // SHIFT everything from 0..(index-1) left by 1 to fill the hole
        shift_left(cache->entries, 0, index - 1);
        // The item is effectively "popped out," so we place it at [0].
        cache->entries[0] = (CacheEntry){rod_length, total_value, cuts};
        return;
    }

    // Not found
    if (cache->size < cache->capacity) {
        // If not full, place new item at index 0, shift old items right
        for (int i = cache->size; i > 0; i--) {
            cache->entries[i] = cache->entries[i-1];
        }
        cache->entries[0].rod_length  = rod_length;
        cache->entries[0].total_value = total_value;
        cache->entries[0].cuts        = cuts;
        cache->size++;
    }
    else {
        // The cache is full, so per MRU, we evict index 0 (the most recently used).
        free_entry(&cache->entries[0]);
        // SHIFT everything left from 1..size-1
        shift_left(cache->entries, 1, cache->size-1);
        // Now the last spot is free. We'll put the new item there (like a LIFO push/pop).
        cache->entries[cache->size-1].rod_length  = rod_length;
        cache->entries[cache->size-1].total_value = total_value;
        cache->entries[cache->size-1].cuts        = cuts;
        // size remains the same
    }
}

/* ----------------- HELPER FUNCTIONS ------------------- */

/**
 * Recursively searches for rod_length in entries[startIndex..size-1].
 * Returns the index if found, -1 if not found.
 */
static int find_index_recursive(Cache* cache, int rod_length, int startIndex)
{
    if (startIndex >= cache->size) {
        return -1;
    }
    if (cache->entries[startIndex].rod_length == rod_length) {
        return startIndex;
    }
    return find_index_recursive(cache, rod_length, startIndex + 1);
}

/**
 * shift_left(entries, fromIndex, toIndex):
 * Recursively shifts the array segment [fromIndex..toIndex] one position to the LEFT.
 *
 * e.g., if fromIndex=0, toIndex=2, we do:
 *   entries[0] = entries[1]
 *   entries[1] = entries[2]
 */
static void shift_left(CacheEntry entries[], int fromIndex, int toIndex)
{
    if (fromIndex >= toIndex) {
        return;
    }
    entries[fromIndex] = entries[fromIndex + 1];
    shift_left(entries, fromIndex + 1, toIndex);
}

/**
 * Frees the dynamically allocated cuts array (if any).
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
