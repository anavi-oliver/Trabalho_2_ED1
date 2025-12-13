#include "sort.h"
#include <stdio.h>
#include <stdlib.h>

// Função auxiliar para copiar array
static void copy_array(void** src, void** dest, int l, int r) {
    for (int i = l; i <= r; i++) {
        dest[i] = src[i];
    }
}

// Intercala dois subarrays ordenados
static void merge(void** arr, int l, int m, int r, Comparator cmp) {
    int n1 = m - l + 1;
    int n2 = r - m;

    // Arrays temporários
    void** L = malloc(n1 * sizeof(void*));
    void** R = malloc(n2 * sizeof(void*));

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    
    while (i < n1 && j < n2) {
        if (cmp(L[i], R[j]) <= 0) {
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
        i++; k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++; k++;
    }

    free(L);
    free(R);
}

void insertion_sort(void** arr, int l, int r, Comparator cmp) {
    for (int i = l + 1; i <= r; i++) {
        void* key = arr[i];
        int j = i - 1;

        while (j >= l && cmp(arr[j], key) > 0) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void merge_sort(void** arr, int l, int r, int threshold, Comparator cmp) {
    if (l < r) {
        // Se o tamanho do vetor for pequeno, usa Insertion Sort
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