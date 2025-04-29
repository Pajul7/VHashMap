#ifndef VHASHMAP_LOADED
#define VHASHMAP_LOADED

#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

typedef struct  {
    char * key;
    void * data;
}t_HashMap_Element ;

t_HashMap_Element * CreateHashMapEl( char * key, void * data );

uint64_t MurmurHash64(char *  key, uint64_t seed);



typedef struct {
    t_HashMap_Element * slots;
    uint64_t seed;
    int size;
} t_HashMap;


t_HashMap * CreateHashMap(int size) ;

void * HM_Get( t_HashMap * HM, char * key);
void HM_Set( t_HashMap * HM, char * key, void * newData);










#endif // VHASHMAP_LOADED
