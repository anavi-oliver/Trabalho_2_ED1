#include "arvore.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct no {
    void *dado;
    struct no *esquerda;
    struct no *direita;
} No;

typedef struct arvore {
    No *raiz;
    int tamanho;
    int (*compar)(const void *, const void *);
} ArvoreStruct;

/*                    FUNÇÕES AUXILIARES                    */

static No* criaNo(void *dado) {
    No *novo = (No*) malloc(sizeof(No));
    if (novo == NULL) {
        fprintf(stderr, "Erro: falha ao alocar nó da árvore.\n");
        return NULL;
    }
    
    novo->dado = dado;
    novo->esquerda = NULL;
    novo->direita = NULL;
    
    return novo;
}

static No* insereNoRec(No *raiz, void *dado, int (*compar)(const void *, const void *), bool *inseriu) {
    if (raiz == NULL) {
        *inseriu = true;
        return criaNo(dado);
    }
    
    int cmp = compar(dado, raiz->dado);
    
    if (cmp < 0) {
        raiz->esquerda = insereNoRec(raiz->esquerda, dado, compar, inseriu);
    } else if (cmp > 0) {
        raiz->direita = insereNoRec(raiz->direita, dado, compar, inseriu);
    } else {
        *inseriu = false;
    }
    
    return raiz;
}

static No* minimoNo(No *no) {
    while (no != NULL && no->esquerda != NULL) {
        no = no->esquerda;
    }
    return no;
}

static No* removeNoRec(No *raiz, void *dado, int (*compar)(const void *, const void *), void **removido) {
    if (raiz == NULL) {
        *removido = NULL;
        return NULL;
    }
    
    int cmp = compar(dado, raiz->dado);
    
    if (cmp < 0) {
        raiz->esquerda = removeNoRec(raiz->esquerda, dado, compar, removido);
    } else if (cmp > 0) {
        raiz->direita = removeNoRec(raiz->direita, dado, compar, removido);
    } else {
        *removido = raiz->dado;
        
        if (raiz->esquerda == NULL) {
            No *temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            No *temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
        
        No *sucessor = minimoNo(raiz->direita);
        raiz->dado = sucessor->dado;
        raiz->direita = removeNoRec(raiz->direita, sucessor->dado, compar, &dado);
        *removido = dado;
    }
    
    return raiz;
}

static void* buscaNoRec(No *raiz, void *dado, int (*compar)(const void *, const void *)) {
    if (raiz == NULL) {
        return NULL;
    }
    
    int cmp = compar(dado, raiz->dado);
    
    if (cmp < 0) {
        return buscaNoRec(raiz->esquerda, dado, compar);
    } else if (cmp > 0) {
        return buscaNoRec(raiz->direita, dado, compar);
    } else {
        return raiz->dado;
    }
}

static void destroiNosRec(No *raiz) {
    if (raiz == NULL) {
        return;
    }
    
    destroiNosRec(raiz->esquerda);
    destroiNosRec(raiz->direita);
    free(raiz);
}

static void destroiNosCompletaRec(No *raiz, void (*destroiDado)(void*)) {
    if (raiz == NULL) {
        return;
    }
    
    destroiNosCompletaRec(raiz->esquerda, destroiDado);
    destroiNosCompletaRec(raiz->direita, destroiDado);
    destroiDado(raiz->dado);
    free(raiz);
}

static int alturaRec(No *raiz) {
    if (raiz == NULL) {
        return 0;
    }
    
    int altEsq = alturaRec(raiz->esquerda);
    int altDir = alturaRec(raiz->direita);
    
    return 1 + ((altEsq > altDir) ? altEsq : altDir);
}

static void percorreEmOrdemRec(No *raiz, void (*funcao)(void*, void*), void *contexto) {
    if (raiz == NULL) {
        return;
    }
    
    percorreEmOrdemRec(raiz->esquerda, funcao, contexto);
    funcao(raiz->dado, contexto);
    percorreEmOrdemRec(raiz->direita, funcao, contexto);
}

static void percorrePreOrdemRec(No *raiz, void (*funcao)(void*, void*), void *contexto) {
    if (raiz == NULL) {
        return;
    }
    
    funcao(raiz->dado, contexto);
    percorrePreOrdemRec(raiz->esquerda, funcao, contexto);
    percorrePreOrdemRec(raiz->direita, funcao, contexto);
}

static void percorrePosOrdemRec(No *raiz, void (*funcao)(void*, void*), void *contexto) {
    if (raiz == NULL) {
        return;
    }
    
    percorrePosOrdemRec(raiz->esquerda, funcao, contexto);
    percorrePosOrdemRec(raiz->direita, funcao, contexto);
    funcao(raiz->dado, contexto);
}

/*                    FUNÇÕES PÚBLICAS                    */

Arvore criaArvore(int (*compar)(const void *, const void *)) {
    if (compar == NULL) {
        return NULL;
    }
    
    ArvoreStruct *arv = (ArvoreStruct*) malloc(sizeof(ArvoreStruct));
    if (arv == NULL) {
        fprintf(stderr, "Erro: falha ao alocar árvore.\n");
        return NULL;
    }
    
    arv->raiz = NULL;
    arv->tamanho = 0;
    arv->compar = compar;
    
    return (Arvore) arv;
}

void destroiArvore(Arvore arv) {
    if (arv == NULL) {
        return;
    }
    
    ArvoreStruct *a = (ArvoreStruct*) arv;
    destroiNosRec(a->raiz);
    free(a);
}

void destroiArvoreCompleta(Arvore arv, void (*destroiDado)(void*)) {
    if (arv == NULL || destroiDado == NULL) {
        return;
    }
    
    ArvoreStruct *a = (ArvoreStruct*) arv;
    destroiNosCompletaRec(a->raiz, destroiDado);
    free(a);
}

bool insereArvore(Arvore arv, void *dado) {
    if (arv == NULL) {
        return false;
    }
    
    ArvoreStruct *a = (ArvoreStruct*) arv;
    bool inseriu = false;
    
    a->raiz = insereNoRec(a->raiz, dado, a->compar, &inseriu);
    
    if (inseriu) {
        a->tamanho++;
    }
    
    return inseriu;
}

void* removeArvore(Arvore arv, void *dado) {
    if (arv == NULL) {
        return NULL;
    }
    
    ArvoreStruct *a = (ArvoreStruct*) arv;
    void *removido = NULL;
    
    a->raiz = removeNoRec(a->raiz, dado, a->compar, &removido);
    
    if (removido != NULL) {
        a->tamanho--;
    }
    
    return removido;
}

void* buscaArvore(Arvore arv, void *dado) {
    if (arv == NULL) {
        return NULL;
    }
    
    ArvoreStruct *a = (ArvoreStruct*) arv;
    return buscaNoRec(a->raiz, dado, a->compar);
}

void* minimoArvore(Arvore arv) {
    if (arv == NULL) {
        return NULL;
    }
    
    ArvoreStruct *a = (ArvoreStruct*) arv;
    No *min = minimoNo(a->raiz);
    
    return (min != NULL) ? min->dado : NULL;
}

void* maximoArvore(Arvore arv) {
    if (arv == NULL) {
        return NULL;
    }
    
    ArvoreStruct *a = (ArvoreStruct*) arv;
    No *no = a->raiz;
    
    while (no != NULL && no->direita != NULL) {
        no = no->direita;
    }
    
    return (no != NULL) ? no->dado : NULL;
}

int tamanhoArvore(Arvore arv) {
    if (arv == NULL) {
        return 0;
    }
    
    ArvoreStruct *a = (ArvoreStruct*) arv;
    return a->tamanho;
}

bool arvoreVazia(Arvore arv) {
    if (arv == NULL) {
        return true;
    }
    
    ArvoreStruct *a = (ArvoreStruct*) arv;
    return a->raiz == NULL;
}

int alturaArvore(Arvore arv) {
    if (arv == NULL) {
        return 0;
    }
    
    ArvoreStruct *a = (ArvoreStruct*) arv;
    return alturaRec(a->raiz);
}

void percorreEmOrdem(Arvore arv, void (*funcao)(void*, void*), void *contexto) {
    if (arv == NULL || funcao == NULL) {
        return;
    }
    
    ArvoreStruct *a = (ArvoreStruct*) arv;
    percorreEmOrdemRec(a->raiz, funcao, contexto);
}

void percorrePreOrdem(Arvore arv, void (*funcao)(void*, void*), void *contexto) {
    if (arv == NULL || funcao == NULL) {
        return;
    }
    
    ArvoreStruct *a = (ArvoreStruct*) arv;
    percorrePreOrdemRec(a->raiz, funcao, contexto);
}

void percorrePosOrdem(Arvore arv, void (*funcao)(void*, void*), void *contexto) {
    if (arv == NULL || funcao == NULL) {
        return;
    }
    
    ArvoreStruct *a = (ArvoreStruct*) arv;
    percorrePosOrdemRec(a->raiz, funcao, contexto);
}