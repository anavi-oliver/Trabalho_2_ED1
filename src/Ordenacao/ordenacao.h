#ifndef ORDENACAO_H
#define ORDENACAO_H

#include <stddef.h>
#include <stdbool.h>

/*
*        MÓDULO DE ORDENAÇÃO UNIFICADO
*
*        Este módulo fornece uma interface unificada para ordenação,
*        permitindo escolher entre qsort (stdlib) e mergesort personalizado.
*        Usado pelo algoritmo de visibilidade conforme parâmetro -to.
*/

/* Tipo de ordenação a ser usado */
typedef enum {
    TIPO_QSORT,      /* qsort da biblioteca padrão */
    TIPO_MERGESORT   /* mergesort personalizado */
} TipoOrdenacao;

/*
Ordena um vetor usando o algoritmo especificado.

Esta é a função principal do módulo, que delega para qsort ou mergesort
de acordo com o tipo especificado.

* base: ponteiro para o início do vetor
* nmemb: número de elementos
* size: tamanho de cada elemento em bytes
* compar: função de comparação
* tipo: TIPO_QSORT ou TIPO_MERGESORT
* limiarInsertionSort: limiar para usar insertion sort (só para mergesort)

Pré-condição: base e compar devem ser válidos
Pós-condição: vetor está ordenado segundo compar
*/
void ordena(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *), TipoOrdenacao tipo, size_t limiarInsertionSort);

/*
Ordena usando qsort da biblioteca padrão.

* base: ponteiro para o início do vetor
* nmemb: número de elementos
* size: tamanho de cada elemento em bytes
* compar: função de comparação

Pré-condição: base e compar devem ser válidos
Pós-condição: vetor está ordenado
*/
void ordenaComQsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));

/*
Ordena usando mergesort híbrido personalizado.

* base: ponteiro para o início do vetor
* nmemb: número de elementos
* size: tamanho de cada elemento em bytes
* compar: função de comparação
* limiarInsertionSort: tamanho máximo para usar insertion sort

Pré-condição: base e compar devem ser válidos
Pós-condição: vetor está ordenado
*/
void ordenaComMergesort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *), size_t limiarInsertionSort);

/*
Converte string do parâmetro -to em tipo de ordenação.

* str: "q" para qsort, "m" para mergesort

Retorna: TIPO_QSORT ou TIPO_MERGESORT (default: TIPO_QSORT)
*/
TipoOrdenacao parseTipoOrdenacao(const char *str);

#endif