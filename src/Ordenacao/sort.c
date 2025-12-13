#include "sort.h"
#include <stdlib.h>

// --- FUNÇÃO AUXILIAR (Mantemos static pois é interna do merge sort) ---

static void merge(void** arr, int l, int m, int r, int (*cmp)(const void*, const void*)) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Aloca arrays temporários
    void** L = malloc(n1 * sizeof(void*));
    void** R = malloc(n2 * sizeof(void*));

    // Copia dados para os arrays temporários
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge dos arrays temporários de volta para arr[l..r]
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2) {
        if (cmp((const void*)&L[i], (const void*)&R[j]) <= 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

// --- INSERTION SORT (Sem 'static', pois está declarada no .h) ---

void insertion_sort(void** arr, int l, int r, int (*cmp)(const void*, const void*)) {
    for (int i = l + 1; i <= r; i++) {
        void* key = arr[i];
        int j = i - 1;

        while (j >= l && cmp((const void*)&arr[j], (const void*)&key) > 0) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// --- MERGE SORT ---

void merge_sort(void** arr, int l, int r, int threshold, int (*cmp)(const void*, const void*)) {
    if (l < r) {
        if ((r - l + 1) <= threshold) {
            insertion_sort(arr, l, r, cmp);
        } else {
            int m = l + (r - l) / 2;

            merge_sort(arr, l, m, threshold, cmp);
            merge_sort(arr, m + 1, r, threshold, cmp);

            merge(arr, l, m, r, cmp);
        }
    }
}