#include "VhashMap.h"

int finalMix64(uint64_t key){ // Fonction finale de mélange de bits

    uint64_t k = key;
    uint64_t magicNumber1 = 0xff51afd7ed558ccd;
    uint64_t magicNumber2 = 0xc4ceb9fe1a85ec53;


    k = k ^ (k >> 33); // décalage de 33 bits vers la droite sur k XOR avec k
    k = k * magicNumber1;
    k = k ^ (k >> 33);
    k = k * magicNumber2;
    k = k ^ (k >> 33);

    return k;
}

uint64_t MurmurHash64(char *  key, uint64_t seed){

    int KeyLength = strlen(key);

    int nBlocs = sizeof(KeyLength) / 16;
    if (sizeof(KeyLength) % 16 != 0) {
        nBlocs += 1;
    }

    // On s'assure que la clé aie une taille multiple de 16 octets
    char * k = malloc( (nBlocs)*16 );
    memset( k , 0, nBlocs*16 );
    strcpy(k,key);


    uint64_t s1 = seed;
    uint64_t s2 = 0;


    uint64_t c1 = 0x87c37b91114253d5; // Des magic numbers pour un mélange de bits plus distribué
    uint64_t c2 = 0x4cf5ad432745937f;

    /// Traitement de la clé

    int offset = 0;

    for ( int b = 0; b < nBlocs; b++ ){ // traitement de chaque bloc complet de 128bits

        offset = b*16;

        uint64_t k1 = * (uint64_t*) (&key + offset); // premiers 64 bits du bloc de 128
        uint64_t k2 = * (uint64_t*) (&key + offset + 8); // derniers 64 bits du bloc de 128

        k1 = k1 * c1;
        k1 = (k1 << 31) | (k1 >> 33);
        k1 = k1 * c2;

        s1 = s1 ^ k1;
        s1 = (s1 << 27) | (s1 >> 37);
        s1 = s1 + s2;
        s1 = s1 * 5 + 0x52dce729;

        k2 = k2 * c2;
        k2 = (k2 << 33) | (k2 >> 31);
        k2 = k2 * c1;

        s2 ^= k2;
        s2 = (s2 << 31) | (s2 >> 33);
        s2 += s1;
        s2 = s2 * 5 + 0x38495ab5;


    }

    s1 = s1 ^ KeyLength;
    s2 = s2 ^ KeyLength;



    s1 += s2;
    s2 += s1;

    s1 = finalMix64(s1);
    s2 = finalMix64(s2);

    s1 = s1 + s2;

    free(k);

    return s1 & 0xFFFFFFFFFFFFFFFF; // garder uniquement les 64bits de poids faible
}

t_HashMap * HM_Create(int size) {




    t_HashMap * HM = malloc(sizeof(t_HashMap));
    HM->size = size;
    HM->seed = rand();
    HM->slots = malloc(sizeof(t_HashMap_Element) * size);


    for (int i = 0 ; i < size; i++){
        (HM->slots)[i].key = NULL;
        (HM->slots)[i].data = NULL;

    }
    return HM;
}



void * HM_Get( t_HashMap * HM, char * key){

    int startIndex = MurmurHash64(key, HM->seed) % HM->size;
    int index = startIndex;

    //printf("initializing checkIndex at %d\n", index);


    do {

        printf("Trying to get item at index : %d\n", index);

        if (    (HM->slots)[index].key == key){
            return (HM->slots)[index].data;

        } else {
            printf("collision !!\n");
        }


        index = (index + 1) % (HM->size);

    } while ( index != startIndex );


    printf("Warning, index not found.\n");
    return NULL;
}


void HM_Set( t_HashMap * HM, char * key, void * newData){

    int startIndex = MurmurHash64(key, HM->seed) % HM->size;
    int index = startIndex;

    //printf("initializing checkIndex at %d\n", index);


    do {

        printf("Trying to set item at index : %d\n", index);

        if (    (HM->slots)[index].key == key || (HM->slots)[index].key == NULL){
            (HM->slots)[index].key = key;
            (HM->slots)[index].data = newData;
            printf("set.\n");
            return;

        } else {
            printf("collision !!\n");
        }


        index = (index + 1) % (HM->size);


    } while ( index != startIndex );


    printf("Warning, index not found.\n");
    return;
}

void HM_Reset( t_HashMap * HM, char * key){

    int startIndex = MurmurHash64(key, HM->seed) % HM->size;
    int index = startIndex;


    do {

        printf("Trying to reset item at index : %d\n", index);

        if ((HM->slots)[index].key == key){
            (HM->slots)[index].key = NULL;
            (HM->slots)[index].data = NULL;
            printf("reset.\n");
            return;

        } else {
            printf("collision !!\n");
        }


        index = (index + 1) % (HM->size);


    } while ( index != startIndex );


    printf("Warning, index not found.\n");
    return;
}




void HM_Destroy( t_HashMap * HM ){

    free( (HM->slots));
    free(HM);
}
