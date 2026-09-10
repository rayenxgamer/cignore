#pragma once

#include <stdlib.h>

#ifdef DARR_STRIP_PREFIX
#define append          arr_append
#define pop             arr_pop
#define remove_by_index arr_remove_by_index
#define remove_by_value arr_remove_by_value
#endif

#define darr_append(a, ...) do { \
    if (a.count >= a.capacity) { \
        if (a.capacity == 0) a.capacity += 256; \
        else a.capacity *= 2;  \
        a.items = realloc(a.items, a.capacity*sizeof(*a.items)); \
    } \
    a.items[a.count++] = __VA_ARGS__; \
} while(0)

#define darr_pop(a) do { \
    if (a.items == NULL){break;} \
    if (a.count > 0) { \
        a.count -= 1; \
    } \
} while(0)

#define darr_remove_by_index(a, idx) do { \
    if (a.count <= 0 || a.capacity <= 0) { \
        printf("could not remove anything from array at %s: %d\n", __FILE__, __LINE__ ); break; \
    }; \
    for (size_t i = idx; i < a.count; i++) { \
        a.items[i] = a.items[(i + 1)]; \
    } \
    a.count--; \
} while(0)

#define darr_remove_by_value(a, val) do { \
    if (a.items == NULL){break;} \
    if (a.count <= 0 || a.capacity <= 0) break; \
    for (int i = 0; i < a.count; i++) { \
        if (a.items[i] == val)  { \
            for (int j = i; j < a.count; j++) { \
                a.items[i] = a.items[(i + 1)%a.count]; \
            } \
            a.count--; \
            printf("index: %d\n", i); \
            break; \
        } \
    } \
} while(0)

#define darr_free(a) do { \
    if (a.items == NULL){break;} \
    free(a.items); \
    a.items = NULL; \
    a.count = a.capacity = 0; \
} while(0)
