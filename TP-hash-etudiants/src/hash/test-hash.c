#include <stdio.h>
#include <string.h>
#include "hash.h"
//#include "hash.inc"
typedef struct {
    char               *mot;
    char               *def;
}
                    Couple;




size_t
    hash_string( char *str)
    {
	unsigned int hash = 0;
	int c;
        c = *str;
	while (c)  {
        c = *str++;
	    hash += c;
}
	return hash;
    }





size_t
mon_hash(const void *p, size_t n)
{
    return hash_string(((Couple *) p)->mot)%n;
}

int
mon_compare(const void *p1, const void *p2)
{
    return strcmp(((Couple *) p1)->mot, ((Couple *) p2)->mot);
}

void
mon_delete(void *data)
{
    free(((Couple *) data)->mot);
    free(((Couple *) data)->def);
}

void
mon_print(void *data, FILE * stream)
{
    fprintf(stream, "%s:%s\n", ((Couple *) data)->mot, ((Couple *) data)->def);
}

int
main()
{
    Couple              c,c1,c2,c3,c4;
    HashTable          *table = hashtable_new(sizeof(Couple), mon_hash, mon_compare);
    
    size_t              i;
    size_t              nb=10;

        c1.mot = malloc(10);
		c1.def = malloc(10);
        strcpy(c1.mot, "mot1");
        strcpy(c1.def, "def1");
    printf("----------\n");

        hashtable_insert(&table, &c1, mon_delete);
hashtable_print(table, mon_print, stdout);
    printf("----------\n")
    ;
        c2.mot = malloc(10);
        c2.def = malloc(10);
        strcpy(c2.mot, "mot2");
        strcpy(c2.def, "def2");
        hashtable_insert(&table, &c2, mon_delete);
	hashtable_print(table, mon_print, stdout);
    printf("----------\n");

        c3.mot = malloc(10);
        c3.def = malloc(10);
    strcpy(c3.mot, "mot3");
        strcpy(c3.def, "def3");
        hashtable_insert(&table, &c3, mon_delete);
	hashtable_print(table, mon_print, stdout);
    
    printf("----------\n");
    c4.mot = malloc(10);
    c4.def = malloc(10);
        strcpy(c4.mot, "mot4");
        strcpy(c4.def, "def4");
        hashtable_insert(&table, &c4, mon_delete);
    hashtable_print(table, mon_print, stdout);
    printf("----------\n");


    hashtable_retract(&table, &c1, mon_delete);
    hashtable_print(table, mon_print, stdout);
    printf("----------\n");
    hashtable_delete(table, mon_delete);
}
