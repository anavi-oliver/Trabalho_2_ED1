#include "mergersort.h"
#include "insertsort.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
Faz o merge (intercalação) de duas metades ordenadas do vetor.
*/
static void merge(unsigned char *arr, size_t inicio, size_t meio, size_t fim,
                  size_t size, int (*compar)(const void *, const void *)) {
    
    size_t n1 = meio - inicio + 1;
    size_t n2 = fim - meio;
    
    unsigned char *esquerda = (unsigned char *)malloc(n1 * size);
    unsigned char *direita = (unsigned char *)malloc(n2 * size);
    
    if (esquerda == NULL || direita == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória no merge.\n");
        free(esquerda);
        free(direita);
        return;
    }
    
    memcpy(esquerda, arr + inicio * size, n1 * size);
    memcpy(direita, arr + (meio + 1) * size, n2 * size);
    
    size_t i = 0, j = 0, k = inicio;
    
    while (i < n1 && j < n2) {
        if (compar(esquerda + i * size, direita + j * size) <= 0) {
            memcpy(arr + k * size, esquerda + i * size, size);
            i++;
        } else {
            memcpy(arr + k * size, direita + j * size, size);
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        memcpy(arr + k * size, esquerda + i * size, size);
        i++;
        k++;
    }
    
    while (j < n2) {
        memcpy(arr + k * size, direita + j * size, size);
        j++;
        k++;
    }
    
    free(esquerda);
    free(direita);
}

/*
Função recursiva auxiliar do merge sort.
*/
static void mergeSortRec(unsigned char *arr, size_t inicio, size_t fim,
                         size_t size, int (*compar)(const void *, const void *)) {
    if (inicio < fim) {
        size_t meio = inicio + (fim - inicio) / 2;
        
        mergeSortRec(arr, inicio, meio, size, compar);
        mergeSortRec(arr, meio + 1, fim, size, compar);
        
        merge(arr, inicio, meio, fim, size, compar);
    }
}

/*
Função recursiva do merge sort híbrido com insertion sort para pequenos vetores.
*/
static void mergeSortHibridoRec(unsigned char *arr, size_t inicio, size_t fim,
                                size_t size, int (*compar)(const void *, const void *),
                                size_t limiar) {
    if (inicio < fim) {
        size_t tamanho = fim - inicio + 1;
        
        if (tamanho <= limiar) {
            insertionSortRange(arr, inicio, fim, size, compar);
        } else {
            size_t meio = inicio + (fim - inicio) / 2;
            
            mergeSortHibridoRec(arr, inicio, meio, size, compar, limiar);
            mergeSortHibridoRec(arr, meio + 1, fim, size, compar, limiar);
            
            merge(arr, inicio, meio, fim, size, compar);
        }
    }
}

void mergeSort(void *base, size_t nmemb, size_t size,
               int (*compar)(const void *, const void *)) {
    if (base == NULL || compar == NULL || nmemb <= 1) {
        return;
    }
    
    unsigned char *arr = (unsigned char *)base;
    mergeSortRec(arr, 0, nmemb - 1, size, compar);
}

void mergeSortHibrido(void *base, size_t nmemb, size_t size,
                      int (*compar)(const void *, const void *),
                      size_t limiarInsertionSort) {
    if (base == NULL || compar == NULL || nmemb <= 1) {
        return;
    }
    
    unsigned char *arr = (unsigned char *)base;
    mergeSortHibridoRec(arr, 0, nmemb - 1, size, compar, limiarInsertionSort);
}