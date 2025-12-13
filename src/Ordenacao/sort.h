#ifndef SORT_H
#define SORT_H

#include <stdlib.h>

// Ponteiro para função de comparação (retorna <0, 0, ou >0)
typedef int (*Comparator)(const void*, const void*);

/*
 * Ordena um array de ponteiros usando Merge Sort.
 * Quando o sub-vetor for menor que 'threshold', usa Insertion Sort.
 * * arr: Array de ponteiros (void**)
 * l, r: índices (0 e n-1)
 * threshold: limite para trocar para insertion sort (padrão do projeto é 10)
 * cmp: função de comparação
 */
void merge_sort(void** arr, int l, int r, int threshold, Comparator cmp);

/*
 * Ordena usando Insertion Sort (usado internamente ou para vetores pequenos)
 */
void insertion_sort(void** arr, int l, int r, Comparator cmp);

#endif