#ifndef MERGERSORT_H
#define MERGERSORT_H


#include <stddef.h>

/*
*        MÓDULO DE MERGE SORT HÍBRIDO
*
*        Implementação do algoritmo Merge Sort com otimização para subvetores
*        pequenos. Quando o tamanho do subvetor é menor ou igual ao limiar,
*        usa Insertion Sort para melhor desempenho.
*/

/*
Ordena um vetor de elementos genéricos usando Merge Sort.

Versão básica sem otimização (sempre usa merge sort).

Complexidade: O(n log n) em todos os casos

* base: ponteiro para o início do vetor
* nmemb: número de elementos no vetor
* size: tamanho de cada elemento em bytes
* compar: função de comparação (retorna <0, 0, >0)

Pré-condição: base e compar devem ser válidos
Pós-condição: vetor ordenado em ordem crescente segundo compar
*/
void mergeSort(void *base, size_t nmemb, size_t size,int (*compar)(const void *, const void *));

/*
Ordena um vetor usando Merge Sort híbrido.

Quando o subvetor tem tamanho <= limiarInsertionSort, usa Insertion Sort.
Esta otimização melhora o desempenho prático do algoritmo.

* base: ponteiro para o início do vetor
* nmemb: número de elementos no vetor
* size: tamanho de cada elemento em bytes
* compar: função de comparação
* limiarInsertionSort: tamanho máximo para usar insertion sort

Pré-condição: base e compar válidos, limiarInsertionSort > 0
Pós-condição: vetor ordenado
*/
void mergeSortHibrido(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *), size_t limiarInsertionSort);

#endif