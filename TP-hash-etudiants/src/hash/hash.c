#include "hash.h"
//#include "hash.inc"
#include <stdlib.h>
#include <stdbool.h>   
#include <string.h> 

const static size_t hashtable_primes[] =
{
 11,
 19,
 37,
 73,
 109,
 163,
 251,
 367,
 557,
 823,
 1237,
 1861,
 2777,
 4177,
 6247,
 9371,
 14057,
 21089,
 31627,
 47431,
 71143,
 106721,
 160073,
 240101,
 360163,
 540217,
 810343,
 1215497,
 1823231,
 2734867,
 4102283,
 6153409,
 9230113,
 13845163,
};
static const double HASHTABLE_MIN_COLLISIONS = 0.3;
static const double HASHTABLE_MAX_COLLISIONS = 3.0;

static const size_t HASHTABLE_NB_PRIMES = sizeof(hashtable_primes) / sizeof(size_t);

#define HASHTABLE_MIN_SIZE hashtable_primes[0]
#define HASHTABLE_MAX_SIZE hashtable_primes[HASHTABLE_NB_PRIMES - 1]

static HashTable   *
_hashtable_new(size_t length, size_t size, size_t(*hash) (const void *, size_t), int (*compare) (const void *, const void *))
{
    HashTable *table = (HashTable *) malloc(sizeof(size_t) * 3 + 2 * sizeof(void *) + sizeof(List) * length);

    if (table) {
        size_t              i;

        table->hash = hash;
        table->compare = compare;
        table->length = length;
        table->nb = 0;
        table->size = size;
        for (i = 0; i < length; i++) {
            table->list[i] = NULL;
        }
    }
    return table;

}




HashTable          *
hashtable_new(size_t size, size_t(*hash) (const void *, size_t), int (*compare) (const void *, const void *))
{
    return _hashtable_new(HASHTABLE_MIN_SIZE, size, hash, compare);
}

static void
_hashtable_resize(HashTable ** ptable)
{
    double              ratio = (*ptable)->nb / (double)(*ptable)->length;

    if ((*ptable)->nb >= HASHTABLE_MIN_SIZE &&
        (*ptable)->nb <= HASHTABLE_MAX_SIZE &&
    (ratio > HASHTABLE_MAX_COLLISIONS || ratio < HASHTABLE_MIN_COLLISIONS)) {
        size_t              i = 1;
        HashTable          *new;

        while (hashtable_primes[i] < (*ptable)->nb)
            i++;

        new = _hashtable_new(hashtable_primes[i],
                             (*ptable)->size,
                             (*ptable)->hash,
                             (*ptable)->compare);

        if (new) {
            size_t              j;

            new->nb = (*ptable)->nb;
            for (j = 0; j < (*ptable)->length; j++) {
                List                list = (*ptable)->list[j], next;

                while (list) {
                    size_t              h = new->hash(list->data, new->length);

                    next = list->next;
                    list->next = new->list[h];
                    new->list[h] = list;
                    list = next;
                }
            }
            free(*ptable);
            *ptable = new;
        }
    }
}

void    hashtable_apply(HashTable * table, void (*func) (void *, void *), void *extra_data) {
    //'func' n'est pas testé dans 'list_foreach'
    //l'utilisation de cette fonction dans le cas d'un NULL sur 'func' n'a pas de sens
    //donc le test à aussi été omis ici pour alerter l'utilisateur par une erreur.
    if (table) {
        list_foreach(*table->list, func, extra_data);
    }
}

void    hashtable_print(HashTable * table, void (*print) (void *, FILE *), FILE * stream) {
    if (table && print && stream) {
        List    next;

        for (int nth = 0; table && nth < table->nb; nth++) {
            next = table->list[nth];
            while (next) {
                (print)(next->data, stream);
                next = next->next;
            }
        }
    }
}

void    hashtable_delete(HashTable * table, void (*delete) (void *)) {
    if (table && delete) {
        List    freeList;
        List    next;

        for (int nth = 0; table && nth < table->nb; nth++) {
            next = table->list[nth];
            while (next) {
                freeList = next;
                (delete)(next->data);
                next = next->next;
                free(freeList);
            }
        }
        free(table);
    }
}

int     hashtable_insert(HashTable ** ptable, void *data, void (*delete) (void *)) {
    if (*ptable && data) {
        const size_t    hashID = (*ptable)->hash(data, (*ptable)->length);
        List            *selectedList;
        bool            exist;

        exist = false;
        selectedList = &(*ptable)->list[hashID];

        //On vérifie que data n'existe pas encore tout en se positionnant sur
        //la dernière liste existante.
        do {
            if (*selectedList) {
                exist = (*ptable)->compare(data, (*selectedList)->data) == 0;
                selectedList = &(*selectedList)->next;
            }
        } while (*selectedList && (*selectedList)->next && !exist);

        //Si la donnée n'existe toujours pas, nous l'ajoutons à la suite
        //de la dernière donnée trouvé (-> la dernière de la liste).
        if (!exist) {
            exist = list_append(&(*selectedList), data, (*ptable)->size);
            if (exist) {
                (*ptable)->nb += 1;
            }

            //Augmente la taille en prévision.
            //Cela n'est pas fait au début de la fonction car nous avons
            //besoin du hash pour trouver l'emplacement avant de voir si la
            //donnée peut ou non être insérer. Dans le cas ou cette donnée ne peux
            //être inséré, il n'est pas nécessaire d'aggrandir la taille.
            if ((*ptable)->nb >= (*ptable)->length) {
                _hashtable_resize(ptable);
            }
        } else if (delete) {
            (*delete)(data);
        }
    }
    return false;
}

int     hashtable_retract(HashTable ** ptable, void *data, void (*delete) (void *)) {
    if (*ptable && data && delete) {
        const size_t    hashID = (*ptable)->hash(data, (*ptable)->length);
        List            *selectedList;
        int             index;

        index = 0;
        selectedList = &(*ptable)->list[hashID];
        while (selectedList && (*ptable)->compare(data, (*selectedList)->data) != 0) {
            selectedList = &(*selectedList)->next;
            index++;
        }

        return list_remove_nth(selectedList, index, delete);
    }
    return false;
}

void    *hashtable_lookup(HashTable * table, void *data) {
    if (table) {
        const size_t    hashID = table->hash(data, table->length);
        List            selectedList;

        selectedList = table->list[hashID];
        return list_find_data(selectedList, data, table->compare);
    }
    return NULL;
}
