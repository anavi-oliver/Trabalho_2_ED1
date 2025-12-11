#ifndef ARVORE_H
#define ARVORE_H

#include <stdbool.h>

/*
*        TIPO ABSTRATO DE DADOS: ÁRVORE BINÁRIA DE BUSCA
*
*        Este módulo implementa uma árvore binária de busca (BST) genérica.
*        Usada para armazenar segmentos ativos no algoritmo de visibilidade.
*        
*        A árvore mantém elementos ordenados segundo uma função de comparação,
*        permitindo busca, inserção e remoção em O(log n) no caso médio.
*/

typedef void *Arvore;

/*                    FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO                    */

/*
Cria uma nova árvore vazia.

* compar: função de comparação para ordenar elementos
          retorna <0 se a < b, 0 se a == b, >0 se a > b

Pré-condição: compar deve ser válida
Pós-condição: retorna uma árvore vazia, ou NULL em caso de falha
*/
Arvore criaArvore(int (*compar)(const void *, const void *));

/*
Libera toda a memória alocada pela árvore.
IMPORTANTE: Não libera os dados armazenados, apenas os nós.

* arv: ponteiro para a árvore

Pré-condição: arv deve ser válida ou NULL
Pós-condição: memória da árvore é liberada
*/
void destroiArvore(Arvore arv);

/*
Libera a árvore e chama função de destruição para cada elemento.

* arv: ponteiro para a árvore
* destroiDado: função que recebe void* e libera o dado

Pré-condição: arv e destroiDado devem ser válidos
Pós-condição: todos os dados e a árvore são liberados
*/
void destroiArvoreCompleta(Arvore arv, void (*destroiDado)(void*));

/*                    OPERAÇÕES DE INSERÇÃO E REMOÇÃO                    */

/*
Insere um elemento na árvore.
Não permite duplicatas (se elemento já existe, não insere).

* arv: ponteiro para a árvore
* dado: ponteiro para o dado a ser inserido

Pré-condição: arv deve ser válida
Pós-condição: elemento inserido se não existia, retorna true se inseriu
*/
bool insereArvore(Arvore arv, void *dado);

/*
Remove um elemento da árvore.
Usa comparação pela função compar fornecida na criação.

* arv: ponteiro para a árvore
* dado: ponteiro para o dado a ser removido

Pré-condição: arv deve ser válida
Pós-condição: elemento removido se existia, retorna o dado ou NULL
*/
void* removeArvore(Arvore arv, void *dado);

/*                    OPERAÇÕES DE BUSCA                    */

/*
Busca um elemento na árvore.

* arv: ponteiro para a árvore
* dado: ponteiro para o dado procurado

Pré-condição: arv deve ser válida
Pós-condição: retorna o elemento se encontrado, NULL caso contrário
*/
void* buscaArvore(Arvore arv, void *dado);

/*
Retorna o menor elemento da árvore (mais à esquerda).

* arv: ponteiro para a árvore

Pré-condição: arv deve ser válida
Pós-condição: retorna o menor elemento, ou NULL se árvore vazia
*/
void* minimoArvore(Arvore arv);

/*
Retorna o maior elemento da árvore (mais à direita).

* arv: ponteiro para a árvore

Pré-condição: arv deve ser válida
Pós-condição: retorna o maior elemento, ou NULL se árvore vazia
*/
void* maximoArvore(Arvore arv);

/*                    OPERAÇÕES DE CONSULTA                    */

/*
Retorna o número de elementos na árvore.

* arv: ponteiro para a árvore

Pré-condição: arv deve ser válida
Pós-condição: retorna o tamanho da árvore
*/
int tamanhoArvore(Arvore arv);

/*
Verifica se a árvore está vazia.

* arv: ponteiro para a árvore

Pré-condição: arv deve ser válida
Pós-condição: retorna true se vazia
*/
bool arvoreVazia(Arvore arv);

/*
Retorna a altura da árvore (número de níveis).

* arv: ponteiro para a árvore

Pré-condição: arv deve ser válida
Pós-condição: retorna a altura (0 se vazia)
*/
int alturaArvore(Arvore arv);

/*                    PERCURSO (TRAVESSIA)                    */

/*
Percorre a árvore em ordem (in-order) aplicando função a cada elemento.
Visita elementos em ordem crescente segundo a função de comparação.

* arv: ponteiro para a árvore
* funcao: função a aplicar (recebe dado e contexto)
* contexto: dado adicional passado à função

Pré-condição: arv e funcao devem ser válidos
Pós-condição: funcao aplicada a todos os elementos em ordem
*/
void percorreEmOrdem(Arvore arv, void (*funcao)(void*, void*), void *contexto);

/*
Percorre a árvore em pré-ordem (pre-order).

* arv: ponteiro para a árvore
* funcao: função a aplicar
* contexto: dado adicional

Pré-condição: arv e funcao devem ser válidos
Pós-condição: funcao aplicada em pré-ordem
*/
void percorrePreOrdem(Arvore arv, void (*funcao)(void*, void*), void *contexto);

/*
Percorre a árvore em pós-ordem (post-order).

* arv: ponteiro para a árvore
* funcao: função a aplicar
* contexto: dado adicional

Pré-condição: arv e funcao devem ser válidos
Pós-condição: funcao aplicada em pós-ordem
*/
void percorrePosOrdem(Arvore arv, void (*funcao)(void*, void*), void *contexto);

#endif