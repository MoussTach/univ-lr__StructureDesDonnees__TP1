#ifndef _HASH_H_
#define _HASH_H_



#include<stdlib.h> 
#include<list.h> 
#include<stdio.h> 


typedef struct {
    size_t              (*hash) (const void *, size_t);
    int                 (*compare) (const void *, const void *);
    size_t              length;
    size_t              nb;
    size_t              size;
    List                list[1];
}
                    HashTable;




HashTable          *
hashtable_new(size_t size, size_t(*hash) (const void *, size_t), int (*compare) (const void *, const void *));


void
hashtable_apply(HashTable * table, void (*func) (void *, void *), void *extra_data);




void
hashtable_print(HashTable * table, void (*print) (void *, FILE *), FILE * stream);



void
hashtable_delete(HashTable * table, void (*delete) (void *));




int
hashtable_insert(HashTable ** ptable, void *data, void (*delete) (void *));

int
hashtable_retract(HashTable ** ptable, void *data, void (*delete) (void *));



void               *
hashtable_lookup(HashTable * table, void *data);
#endif
