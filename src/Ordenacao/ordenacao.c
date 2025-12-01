#include "ordenacao.h"
#include "mergersort.h"
#include <stdlib.h>
#include <string.h>

void ordena(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *),TipoOrdenacao tipo, size_t limiarInsertionSort) {
    
    if (base == NULL || compar == NULL) {
        return;
    }
    
    switch (tipo) {
        case TIPO_QSORT:
            ordenaComQsort(base, nmemb, size, compar);
            break;
            
        case TIPO_MERGESORT:
            ordenaComMergesort(base, nmemb, size, compar, limiarInsertionSort);
            break;
            
        default:
            ordenaComQsort(base, nmemb, size, compar);
            break;
    }
}

void ordenaComQsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
    if (base == NULL || compar == NULL) {
        return;
    }
    
    qsort(base, nmemb, size, compar);
}

void ordenaComMergesort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *), size_t limiarInsertionSort) {
    if (base == NULL || compar == NULL) {
        return;
    }
    
    mergeSortHibrido(base, nmemb, size, compar, limiarInsertionSort);
}

TipoOrdenacao parseTipoOrdenacao(const char *str) {
    if (str == NULL) {
        return TIPO_QSORT;
    }
    
    if (strcmp(str, "q") == 0 || strcmp(str, "Q") == 0) {
        return TIPO_QSORT;
    }
    
    if (strcmp(str, "m") == 0 || strcmp(str, "M") == 0) {
        return TIPO_MERGESORT;
    }
    
    return TIPO_QSORT;
}