#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>

/*
*        TIPO ABSTRATO DE DADOS: LISTA DUPLAMENTE ENCADEADA
*
*        Este módulo define uma lista genérica duplamente encadeada.
*        A lista pode armazenar ponteiros para qualquer tipo de dado (void*).
*        A representação interna é escondida por um ponteiro opaco.
*/

typedef void *Lista;

/*                    FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO                    */

/*
Cria uma nova lista vazia.

Pré-condição: nenhuma
Pós-condição: retorna um ponteiro opaco para a lista criada,
              ou NULL em caso de falha
*/
Lista criaLista();

/*
Libera toda a memória alocada pela lista.
IMPORTANTE: Não libera os dados armazenados, apenas os nós da lista.
Se você precisa liberar os dados, percorra a lista e libere cada elemento
antes de chamar esta função.

* l: ponteiro para a lista

Pré-condição: l deve ser válido ou NULL
Pós-condição: memória da lista é liberada
*/
void destroiLista(Lista l);

/*
Libera a lista e chama uma função de destruição para cada elemento.

* l: ponteiro para a lista
* destroiDado: função que recebe um void* e libera o dado

Pré-condição: l e destroiDado devem ser válidos
Pós-condição: todos os dados são destruídos e a lista é liberada
*/
void destroiListaCompleta(Lista l, void (*destroiDado)(void*));

/*                    OPERAÇÕES DE INSERÇÃO                    */

/*
Insere um elemento no início da lista.

* l: ponteiro para a lista
* dado: ponteiro para o dado a ser inserido

Pré-condição: l deve ser válido
Pós-condição: elemento inserido no início, retorna true se sucesso
*/
bool insereListaInicio(Lista l, void *dado);

/*
Insere um elemento no final da lista.

* l: ponteiro para a lista
* dado: ponteiro para o dado a ser inserido

Pré-condição: l deve ser válido
Pós-condição: elemento inserido no final, retorna true se sucesso
*/
bool insereListaFim(Lista l, void *dado);

/*
Insere um elemento em uma posição específica (0-indexado).

* l: ponteiro para a lista
* dado: ponteiro para o dado a ser inserido
* pos: posição de inserção (0 = início)

Pré-condição: l deve ser válido
Pós-condição: elemento inserido na posição, retorna true se sucesso
*/
bool insereListaPosicao(Lista l, void *dado, int pos);

/*                    OPERAÇÕES DE REMOÇÃO                    */

/*
Remove e retorna o elemento do início da lista.

* l: ponteiro para a lista

Pré-condição: l deve ser válido
Pós-condição: primeiro elemento é removido e retornado,
              ou NULL se lista vazia
*/
void* removeListaInicio(Lista l);

/*
Remove e retorna o elemento do final da lista.

* l: ponteiro para a lista

Pré-condição: l deve ser válido
Pós-condição: último elemento é removido e retornado,
              ou NULL se lista vazia
*/
void* removeListaFim(Lista l);

/*
Remove e retorna o elemento de uma posição específica.

* l: ponteiro para a lista
* pos: posição do elemento (0-indexado)

Pré-condição: l deve ser válido
Pós-condição: elemento na posição é removido e retornado,
              ou NULL se posição inválida
*/
void* removeListaPosicao(Lista l, int pos);

/*
Remove a primeira ocorrência de um elemento da lista.
Usa comparação por ponteiro (==).

* l: ponteiro para a lista
* dado: ponteiro para o dado a ser removido

Pré-condição: l deve ser válido
Pós-condição: primeira ocorrência é removida, retorna true se encontrado
*/
bool removeListaElemento(Lista l, void *dado);

/*                    OPERAÇÕES DE CONSULTA                    */

/*
Retorna o elemento em uma posição específica sem removê-lo.

* l: ponteiro para a lista
* pos: posição do elemento (0-indexado)

Pré-condição: l deve ser válido
Pós-condição: retorna o elemento na posição, ou NULL se inválida
*/
void* getListaPosicao(Lista l, int pos);

/*
Retorna o primeiro elemento da lista sem removê-lo.

* l: ponteiro para a lista

Pré-condição: l deve ser válido
Pós-condição: retorna o primeiro elemento, ou NULL se vazia
*/
void* getListaInicio(Lista l);

/*
Retorna o último elemento da lista sem removê-lo.

* l: ponteiro para a lista

Pré-condição: l deve ser válido
Pós-condição: retorna o último elemento, ou NULL se vazia
*/
void* getListaFim(Lista l);

/*
Retorna o número de elementos na lista.

* l: ponteiro para a lista

Pré-condição: l deve ser válido
Pós-condição: retorna o tamanho da lista
*/
int tamanhoLista(Lista l);

/*
Verifica se a lista está vazia.

* l: ponteiro para a lista

Pré-condição: l deve ser válido
Pós-condição: retorna true se a lista está vazia
*/
bool listaVazia(Lista l);

/*
Busca um elemento na lista usando comparação por ponteiro.

* l: ponteiro para a lista
* dado: ponteiro para o dado procurado

Pré-condição: l deve ser válido
Pós-condição: retorna true se o elemento foi encontrado
*/
bool buscaLista(Lista l, void *dado);

/*                    ITERAÇÃO                    */

/*
Aplica uma função a todos os elementos da lista.

* l: ponteiro para a lista
* funcao: função que recebe um void* e um void* (contexto adicional)
* contexto: dado adicional passado para a função

Pré-condição: l e funcao devem ser válidos
Pós-condição: funcao é aplicada a cada elemento
*/
void percorreLista(Lista l, void (*funcao)(void*, void*), void *contexto);

#endif