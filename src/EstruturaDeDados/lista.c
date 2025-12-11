#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct no {
    void *dado;
    struct no *proximo;
    struct no *anterior;
} No;

typedef struct lista {
    No *inicio;
    No *fim;
    int tamanho;
} ListaStruct;

/*                    FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO                    */

Lista criaLista() {
    ListaStruct *l = (ListaStruct*) malloc(sizeof(ListaStruct));
    if (l == NULL) {
        fprintf(stderr, "Erro: falha na alocação de memória para lista.\n");
        return NULL;
    }
    
    l->inicio = NULL;
    l->fim = NULL;
    l->tamanho = 0;
    
    return (Lista) l;
}

void destroiLista(Lista l) {
    if (l == NULL) {
        return;
    }
    
    ListaStruct *lista = (ListaStruct*) l;
    No *atual = lista->inicio;
    
    while (atual != NULL) {
        No *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    
    free(lista);
}

void destroiListaCompleta(Lista l, void (*destroiDado)(void*)) {
    if (l == NULL || destroiDado == NULL) {
        return;
    }
    
    ListaStruct *lista = (ListaStruct*) l;
    No *atual = lista->inicio;
    
    while (atual != NULL) {
        No *proximo = atual->proximo;
        destroiDado(atual->dado);
        free(atual);
        atual = proximo;
    }
    
    free(lista);
}

/*                    OPERAÇÕES DE INSERÇÃO                    */

bool insereListaInicio(Lista l, void *dado) {
    if (l == NULL) {
        return false;
    }
    
    ListaStruct *lista = (ListaStruct*) l;
    No *novo = (No*) malloc(sizeof(No));
    
    if (novo == NULL) {
        fprintf(stderr, "Erro: falha na alocação de nó da lista.\n");
        return false;
    }
    
    novo->dado = dado;
    novo->proximo = lista->inicio;
    novo->anterior = NULL;
    
    if (lista->inicio != NULL) {
        lista->inicio->anterior = novo;
    } else {
        lista->fim = novo;
    }
    
    lista->inicio = novo;
    lista->tamanho++;
    
    return true;
}

bool insereListaFim(Lista l, void *dado) {
    if (l == NULL) {
        return false;
    }
    
    ListaStruct *lista = (ListaStruct*) l;
    No *novo = (No*) malloc(sizeof(No));
    
    if (novo == NULL) {
        fprintf(stderr, "Erro: falha na alocação de nó da lista.\n");
        return false;
    }
    
    novo->dado = dado;
    novo->proximo = NULL;
    novo->anterior = lista->fim;
    
    if (lista->fim != NULL) {
        lista->fim->proximo = novo;
    } else {
        lista->inicio = novo;
    }
    
    lista->fim = novo;
    lista->tamanho++;
    
    return true;
}

bool insereListaPosicao(Lista l, void *dado, int pos) {
    if (l == NULL || pos < 0) {
        return false;
    }
    
    ListaStruct *lista = (ListaStruct*) l;
    
    if (pos == 0) {
        return insereListaInicio(l, dado);
    }
    
    if (pos >= lista->tamanho) {
        return insereListaFim(l, dado);
    }
    
    No *atual = lista->inicio;
    int i;
    for (i = 0; i < pos && atual != NULL; i++) {
        atual = atual->proximo;
    }
    
    if (atual == NULL) {
        return false;
    }
    
    No *novo = (No*) malloc(sizeof(No));
    if (novo == NULL) {
        fprintf(stderr, "Erro: falha na alocação de nó da lista.\n");
        return false;
    }
    
    novo->dado = dado;
    novo->proximo = atual;
    novo->anterior = atual->anterior;
    
    if (atual->anterior != NULL) {
        atual->anterior->proximo = novo;
    }
    atual->anterior = novo;
    
    lista->tamanho++;
    
    return true;
}

/*                    OPERAÇÕES DE REMOÇÃO                    */

void* removeListaInicio(Lista l) {
    if (l == NULL) {
        return NULL;
    }
    
    ListaStruct *lista = (ListaStruct*) l;
    
    if (lista->inicio == NULL) {
        return NULL;
    }
    
    No *removido = lista->inicio;
    void *dado = removido->dado;
    
    lista->inicio = removido->proximo;
    
    if (lista->inicio != NULL) {
        lista->inicio->anterior = NULL;
    } else {
        lista->fim = NULL;
    }
    
    free(removido);
    lista->tamanho--;
    
    return dado;
}

void* removeListaFim(Lista l) {
    if (l == NULL) {
        return NULL;
    }
    
    ListaStruct *lista = (ListaStruct*) l;
    
    if (lista->fim == NULL) {
        return NULL;
    }
    
    No *removido = lista->fim;
    void *dado = removido->dado;
    
    lista->fim = removido->anterior;
    
    if (lista->fim != NULL) {
        lista->fim->proximo = NULL;
    } else {
        lista->inicio = NULL;
    }
    
    free(removido);
    lista->tamanho--;
    
    return dado;
}

void* removeListaPosicao(Lista l, int pos) {
    if (l == NULL || pos < 0) {
        return NULL;
    }
    
    ListaStruct *lista = (ListaStruct*) l;
    
    if (pos == 0) {
        return removeListaInicio(l);
    }
    
    if (pos >= lista->tamanho) {
        return NULL;
    }
    
    No *atual = lista->inicio;
    int i;
    for (i = 0; i < pos && atual != NULL; i++) {
        atual = atual->proximo;
    }
    
    if (atual == NULL) {
        return NULL;
    }
    
    void *dado = atual->dado;
    
    if (atual->anterior != NULL) {
        atual->anterior->proximo = atual->proximo;
    }
    
    if (atual->proximo != NULL) {
        atual->proximo->anterior = atual->anterior;
    }
    
    if (atual == lista->fim) {
        lista->fim = atual->anterior;
    }
    
    free(atual);
    lista->tamanho--;
    
    return dado;
}

bool removeListaElemento(Lista l, void *dado) {
    if (l == NULL) {
        return false;
    }
    
    ListaStruct *lista = (ListaStruct*) l;
    No *atual = lista->inicio;
    
    while (atual != NULL) {
        if (atual->dado == dado) {
            if (atual->anterior != NULL) {
                atual->anterior->proximo = atual->proximo;
            } else {
                lista->inicio = atual->proximo;
            }
            
            if (atual->proximo != NULL) {
                atual->proximo->anterior = atual->anterior;
            } else {
                lista->fim = atual->anterior;
            }
            
            free(atual);
            lista->tamanho--;
            return true;
        }
        atual = atual->proximo;
    }
    
    return false;
}

/*                    OPERAÇÕES DE CONSULTA                    */

void* getListaPosicao(Lista l, int pos) {
    if (l == NULL || pos < 0) {
        return NULL;
    }
    
    ListaStruct *lista = (ListaStruct*) l;
    
    if (pos >= lista->tamanho) {
        return NULL;
    }
    
    No *atual = lista->inicio;
    int i;
    for (i = 0; i < pos && atual != NULL; i++) {
        atual = atual->proximo;
    }
    
    return (atual != NULL) ? atual->dado : NULL;
}

void* getListaInicio(Lista l) {
    if (l == NULL) {
        return NULL;
    }
    
    ListaStruct *lista = (ListaStruct*) l;
    return (lista->inicio != NULL) ? lista->inicio->dado : NULL;
}

void* getListaFim(Lista l) {
    if (l == NULL) {
        return NULL;
    }
    
    ListaStruct *lista = (ListaStruct*) l;
    return (lista->fim != NULL) ? lista->fim->dado : NULL;
}

int tamanhoLista(Lista l) {
    if (l == NULL) {
        return 0;
    }
    
    ListaStruct *lista = (ListaStruct*) l;
    return lista->tamanho;
}

bool listaVazia(Lista l) {
    if (l == NULL) {
        return true;
    }
    
    ListaStruct *lista = (ListaStruct*) l;
    return lista->tamanho == 0;
}

bool buscaLista(Lista l, void *dado) {
    if (l == NULL) {
        return false;
    }
    
    ListaStruct *lista = (ListaStruct*) l;
    No *atual = lista->inicio;
    
    while (atual != NULL) {
        if (atual->dado == dado) {
            return true;
        }
        atual = atual->proximo;
    }
    
    return false;
}

/*                    ITERAÇÃO                    */

void percorreLista(Lista l, void (*funcao)(void*, void*), void *contexto) {
    if (l == NULL || funcao == NULL) {
        return;
    }
    
    ListaStruct *lista = (ListaStruct*) l;
    No *atual = lista->inicio;
    
    while (atual != NULL) {
        funcao(atual->dado, contexto);
        atual = atual->proximo;
    }
}