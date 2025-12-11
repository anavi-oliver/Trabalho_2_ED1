#include "insertsort.h"
#include <string.h>
#include <stdlib.h>

/*
Troca dois elementos de tamanho arbitrário usando buffer temporário.
*/
static void trocar(void *a, void *b, size_t size) {
    unsigned char temp[size];
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

void insertionSort(void *base, size_t nmemb, size_t size,
                   int (*compar)(const void *, const void *)) {
    if (base == NULL || compar == NULL || nmemb <= 1) {
        return;
    }
    
    unsigned char *arr = (unsigned char *)base;
    size_t i, j;
    
    for (i = 1; i < nmemb; i++) {
        j = i;
        
        while (j > 0 && compar(arr + j * size, arr + (j - 1) * size) < 0) {
            trocar(arr + j * size, arr + (j - 1) * size, size);
            j--;
        }
    }
}

void insertionSortRange(void *base, size_t inicio, size_t fim, 
                        size_t size, int (*compar)(const void *, const void *)) {
    if (base == NULL || compar == NULL || inicio >= fim) {
        return;
    }
    
    unsigned char *arr = (unsigned char *)base;
    size_t i, j;
    
    for (i = inicio + 1; i <= fim; i++) {
        j = i;
        
        while (j > inicio && compar(arr + j * size, arr + (j - 1) * size) < 0) {
            trocar(arr + j * size, arr + (j - 1) * size, size);
            j--;
        }
    }
}