#include "segsativos.h"
#include "segmento.h"
#include "ponto.h"  
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Nó da árvore binária de busca
typedef struct no_segmento {
    void* segmento;                 // Ponteiro para o segmento
    double distancia;               // Distância ao ponto de observação
    struct no_segmento* esquerda;   // Subárvore esquerda (segmentos mais próximos)
    struct no_segmento* direita;    // Subárvore direita (segmentos mais distantes)
} NoSegmentoInterno;

// Estrutura principal da árvore
typedef struct {
    NoSegmentoInterno* raiz;
    double px;          // Coordenada X do ponto de observação
    double py;          // Coordenada Y do ponto de observação
    int tamanho;        // Número de segmentos na árvore
} SegmentosAtivosInterno;


/*________________________________ FUNÇÕES AUXILIARES INTERNAS ________________________________*/

// Calcula a distância do ponto (px, py) ao segmento ao longo de um raio com ângulo dado
static double calculaDistanciaRaioSegmento(double px, double py, void* segmento, double angulo) {
    if (segmento == NULL) {
        return INFINITY;
    }
    
    // Direção do raio
    double dx_raio = cos(angulo);
    double dy_raio = sin(angulo);
    
    // Coordenadas do segmento
Ponto p1 = getPonto1Segmento(segmento);
Ponto p2 = getPonto2Segmento(segmento);
double x1 = getXPonto(p1);
double y1 = getYPonto(p1);
double x2 = getXPonto(p2);
double y2 = getYPonto(p2);
    
    // Vetor do segmento
    double dx_seg = x2 - x1;
    double dy_seg = y2 - y1;
    
    // Vetor do ponto ao início do segmento
    double dx_p1 = x1 - px;
    double dy_p1 = y1 - py;
    
    // Calcula interseção raio-segmento usando determinantes
    double det = dx_raio * dy_seg - dy_raio * dx_seg;
    
    if (fabs(det) < 1e-10) {
        // Raio e segmento são paralelos
        return INFINITY;
    }
    
    double t = (dx_p1 * dy_seg - dy_p1 * dx_seg) / det;
    double u = (dx_p1 * dy_raio - dy_p1 * dx_raio) / det;
    
    // Verifica se a interseção está no raio (t >= 0) e no segmento (0 <= u <= 1)
    if (t >= 0 && u >= 0 && u <= 1) {
        return t;  // Distância ao longo do raio
    }
    
    return INFINITY;
}

// Cria um novo nó
static NoSegmentoInterno* criaNo(void* segmento, double distancia) {
    NoSegmentoInterno* no = (NoSegmentoInterno*)malloc(sizeof(NoSegmentoInterno));
    if (no == NULL) {
        fprintf(stderr, "ERRO: Falha ao alocar memória para nó de segmento.\n");
        return NULL;
    }
    
    no->segmento = segmento;
    no->distancia = distancia;
    no->esquerda = NULL;
    no->direita = NULL;
    
    return no;
}

// Insere um nó na árvore (recursivo)
static NoSegmentoInterno* insereNoRecursivo(NoSegmentoInterno* raiz, NoSegmentoInterno* novo) {
    if (raiz == NULL) {
        return novo;
    }
    
    // Segmentos mais próximos vão para a esquerda
    if (novo->distancia < raiz->distancia) {
        raiz->esquerda = insereNoRecursivo(raiz->esquerda, novo);
    } else {
        raiz->direita = insereNoRecursivo(raiz->direita, novo);
    }
    
    return raiz;
}

// Encontra o nó com o menor valor (mais à esquerda)
static NoSegmentoInterno* encontraMinimo(NoSegmentoInterno* no) {
    while (no != NULL && no->esquerda != NULL) {
        no = no->esquerda;
    }
    return no;
}

// Remove um nó da árvore (recursivo)
static NoSegmentoInterno* removeNoRecursivo(NoSegmentoInterno* raiz, void* segmento, bool* removido) {
    if (raiz == NULL) {
        return NULL;
    }
    
    if (raiz->segmento == segmento) {
        *removido = true;
        
        // Caso 1: Nó folha
        if (raiz->esquerda == NULL && raiz->direita == NULL) {
            free(raiz);
            return NULL;
        }
        
        // Caso 2: Nó com um filho
        if (raiz->esquerda == NULL) {
            NoSegmentoInterno* temp = raiz->direita;
            free(raiz);
            return temp;
        }
        if (raiz->direita == NULL) {
            NoSegmentoInterno* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
        
        // Caso 3: Nó com dois filhos
        NoSegmentoInterno* sucessor = encontraMinimo(raiz->direita);
        raiz->segmento = sucessor->segmento;
        raiz->distancia = sucessor->distancia;
        raiz->direita = removeNoRecursivo(raiz->direita, sucessor->segmento, removido);
        *removido = true;
        
        return raiz;
    }
    
    // Busca recursiva
    raiz->esquerda = removeNoRecursivo(raiz->esquerda, segmento, removido);
    if (!(*removido)) {
        raiz->direita = removeNoRecursivo(raiz->direita, segmento, removido);
    }
    
    return raiz;
}

// Libera todos os nós da árvore (recursivo)
static void destroiNosRecursivo(NoSegmentoInterno* raiz) {
    if (raiz == NULL) {
        return;
    }
    destroiNosRecursivo(raiz->esquerda);
    destroiNosRecursivo(raiz->direita);
    free(raiz);
}

// Imprime a árvore (recursivo, em ordem)
static void imprimeNosRecursivo(NoSegmentoInterno* raiz, FILE* arquivo, int nivel) {
    if (raiz == NULL) {
        return;
    }
    
    imprimeNosRecursivo(raiz->direita, arquivo, nivel + 1);
    
    for (int i = 0; i < nivel; i++) {
        fprintf(arquivo, "    ");
    }
    fprintf(arquivo, "Dist: %.2f\n", raiz->distancia);
    
    imprimeNosRecursivo(raiz->esquerda, arquivo, nivel + 1);
}


/*________________________________ FUNÇÕES PÚBLICAS ________________________________*/

SegmentosAtivos criaSegmentosAtivos(double px, double py) {
    SegmentosAtivosInterno* sa = (SegmentosAtivosInterno*)malloc(sizeof(SegmentosAtivosInterno));
    if (sa == NULL) {
        fprintf(stderr, "ERRO: Falha ao alocar memória para árvore de segmentos ativos.\n");
        return NULL;
    }
    
    sa->raiz = NULL;
    sa->px = px;
    sa->py = py;
    sa->tamanho = 0;
    
    return (SegmentosAtivos)sa;
}

void destroiSegmentosAtivos(SegmentosAtivos sa) {
    if (sa == NULL) {
        return;
    }
    
    SegmentosAtivosInterno* arvore = (SegmentosAtivosInterno*)sa;
    destroiNosRecursivo(arvore->raiz);
    free(arvore);
}

void insereSegmentoAtivo(SegmentosAtivos sa, void* segmento, double anguloVarredura) {
    if (sa == NULL || segmento == NULL) {
        return;
    }
    
    SegmentosAtivosInterno* arvore = (SegmentosAtivosInterno*)sa;
    
    double distancia = calculaDistanciaRaioSegmento(arvore->px, arvore->py, segmento, anguloVarredura);
    
    if (distancia == INFINITY) {
        return;  // Segmento não intersecta o raio
    }
    
    NoSegmentoInterno* novo = criaNo(segmento, distancia);
    if (novo == NULL) {
        return;
    }
    
    arvore->raiz = insereNoRecursivo(arvore->raiz, novo);
    arvore->tamanho++;
}

bool removeSegmentoAtivo(SegmentosAtivos sa, void* segmento) {
    if (sa == NULL || segmento == NULL) {
        return false;
    }
    
    SegmentosAtivosInterno* arvore = (SegmentosAtivosInterno*)sa;
    bool removido = false;
    
    arvore->raiz = removeNoRecursivo(arvore->raiz, segmento, &removido);
    
    if (removido) {
        arvore->tamanho--;
    }
    
    return removido;
}

void* getSegmentoMaisProximo(const SegmentosAtivos sa, double anguloVarredura) {
     (void)anguloVarredura; 
    if (sa == NULL) {
        return NULL;
    }
    
    SegmentosAtivosInterno* arvore = (SegmentosAtivosInterno*)sa;
    
    if (arvore->raiz == NULL) {
        return NULL;
    }
    
    // O segmento mais próximo está no nó mais à esquerda
    NoSegmentoInterno* atual = arvore->raiz;
    while (atual->esquerda != NULL) {
        atual = atual->esquerda;
    }
    
    return atual->segmento;
}

bool segmentosAtivosVazio(const SegmentosAtivos sa) {
    if (sa == NULL) {
        return true;
    }
    
    SegmentosAtivosInterno* arvore = (SegmentosAtivosInterno*)sa;
    return (arvore->raiz == NULL);
}

int getNumeroSegmentosAtivos(const SegmentosAtivos sa) {
    if (sa == NULL) {
        return 0;
    }
    
    SegmentosAtivosInterno* arvore = (SegmentosAtivosInterno*)sa;
    return arvore->tamanho;
}

void getPontoObservacao(const SegmentosAtivos sa, double* px, double* py) {
    if (sa == NULL || px == NULL || py == NULL) {
        return;
    }
    
    SegmentosAtivosInterno* arvore = (SegmentosAtivosInterno*)sa;
    *px = arvore->px;
    *py = arvore->py;
}

void imprimeSegmentosAtivos(const SegmentosAtivos sa, FILE* arquivo) {
    if (sa == NULL || arquivo == NULL) {
        return;
    }
    
    SegmentosAtivosInterno* arvore = (SegmentosAtivosInterno*)sa;
    
    fprintf(arquivo, "=== Árvore de Segmentos Ativos ===\n");
    fprintf(arquivo, "Ponto de observação: (%.2f, %.2f)\n", arvore->px, arvore->py);
    fprintf(arquivo, "Número de segmentos: %d\n", arvore->tamanho);
    
    if (arvore->raiz == NULL) {
        fprintf(arquivo, "(Árvore vazia)\n");
    } else {
        imprimeNosRecursivo(arvore->raiz, arquivo, 0);
    }
    
    fprintf(arquivo, "===================================\n");
}