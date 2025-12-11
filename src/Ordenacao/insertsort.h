#ifndef INSERTSORT_H
#define INSERTSORT_H

#include <stddef.h>

/*
*        MÓDULO DE INSERTION SORT
*
*        Este módulo implementa o algoritmo de ordenação Insertion Sort.
*        É eficiente para vetores pequenos ou quase ordenados.
*        Usado como otimização quando subvetores ficam pequenos no mergesort.
*/

/*
Ordena um vetor de elementos genéricos usando Insertion Sort.

O algoritmo percorre o vetor da esquerda para a direita, inserindo
cada elemento na posição correta dentro da parte já ordenada.

Complexidade: O(n²) no pior caso, O(n) no melhor caso (vetor já ordenado)

* base: ponteiro para o início do vetor
* nmemb: número de elementos no vetor
* size: tamanho de cada elemento em bytes
* compar: função de comparação (retorna <0, 0, >0)

Pré-condição: base e compar devem ser válidos
Pós-condição: vetor ordenado em ordem crescente segundo compar
*/
void insertionSort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));

/*
Ordena uma região específica de um vetor usando Insertion Sort.
Útil para ordenar subvetores no mergesort.

* base: ponteiro para o início do vetor
* inicio: índice inicial da região a ordenar
* fim: índice final (inclusivo) da região a ordenar
* size: tamanho de cada elemento em bytes
* compar: função de comparação

Pré-condição: base e compar válidos, 0 <= inicio <= fim < nmemb
Pós-condição: região [inicio, fim] está ordenada
*/
void insertionSortRange(void *base, size_t inicio, size_t fim, size_t size, int (*compar)(const void *, const void *));

#endif