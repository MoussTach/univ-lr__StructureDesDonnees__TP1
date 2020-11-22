
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <list.h>

typedef struct {
    size_t              (*hash) (const void *, size_t);
    int                 (*compare) (const void *, const void *);
    size_t              length;
    size_t              nb;
    size_t              size;
    List                list[1];
}
                    HashTable;

