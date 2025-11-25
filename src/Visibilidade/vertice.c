#include "vertice.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.14159265358979323846


// Estrutura interna do vértice
typedef struct vertice {
    double x;
    double y;
    TipoVertice tipo;
    double angulo;
    void* segmento;  // Ponteiro para o segmento associado
} VerticeInterno;


/*________________________________ FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO ________________________________*/

Vertice criaVertice(double x, double y, TipoVertice tipo, double angulo, void* segmento) {
    VerticeInterno* v = (VerticeInterno*)malloc(sizeof(VerticeInterno));
    if (v == NULL) {
        fprintf(stderr, "ERRO: Falha ao alocar memória para vértice.\n");
        return NULL;
    }
    
    v->x = x;
    v->y = y;
    v->tipo = tipo;
    v->angulo = angulo;
    v->segmento = segmento;
    
    return (Vertice)v;
}

void destroiVertice(Vertice v) {
    if (v == NULL) {
        return;
    }
    free(v);
}


/*________________________________ MÉTODOS GET (CONSULTA) ________________________________*/

double getVerticeX(const Vertice v) {
    if (v == NULL) {
        return 0.0;
    }
    VerticeInterno* vert = (VerticeInterno*)v;
    return vert->x;
}

double getVerticeY(const Vertice v) {
    if (v == NULL) {
        return 0.0;
    }
    VerticeInterno* vert = (VerticeInterno*)v;
    return vert->y;
}

TipoVertice getVerticeTipo(const Vertice v) {
    if (v == NULL) {
        return TIPO_INICIO;  // Valor padrão
    }
    VerticeInterno* vert = (VerticeInterno*)v;
    return vert->tipo;
}

double getVerticeAngulo(const Vertice v) {
    if (v == NULL) {
        return 0.0;
    }
    VerticeInterno* vert = (VerticeInterno*)v;
    return vert->angulo;
}

void* getVerticeSegmento(const Vertice v) {
    if (v == NULL) {
        return NULL;
    }
    VerticeInterno* vert = (VerticeInterno*)v;
    return vert->segmento;
}


/*________________________________ MÉTODOS SET (MODIFICAÇÃO) ________________________________*/

void setVerticeX(Vertice v, double x) {
    if (v == NULL) {
        return;
    }
    VerticeInterno* vert = (VerticeInterno*)v;
    vert->x = x;
}

void setVerticeY(Vertice v, double y) {
    if (v == NULL) {
        return;
    }
    VerticeInterno* vert = (VerticeInterno*)v;
    vert->y = y;
}

void setVerticeTipo(Vertice v, TipoVertice tipo) {
    if (v == NULL) {
        return;
    }
    VerticeInterno* vert = (VerticeInterno*)v;
    vert->tipo = tipo;
}

void setVerticeAngulo(Vertice v, double angulo) {
    if (v == NULL) {
        return;
    }
    VerticeInterno* vert = (VerticeInterno*)v;
    vert->angulo = angulo;
}

void setVerticeSegmento(Vertice v, void* segmento) {
    if (v == NULL) {
        return;
    }
    VerticeInterno* vert = (VerticeInterno*)v;
    vert->segmento = segmento;
}


/*________________________________ FUNÇÕES UTILITÁRIAS ________________________________*/

int comparaVerticesPorAngulo(const void* v1, const void* v2) {
    const Vertice* vert1 = (const Vertice*)v1;
    const Vertice* vert2 = (const Vertice*)v2;
    
    double ang1 = getVerticeAngulo(*vert1);
    double ang2 = getVerticeAngulo(*vert2);
    
    // Comparação por ângulo
    if (ang1 < ang2) return -1;
    if (ang1 > ang2) return 1;
    
    // Se ângulos iguais, vértices de FIM vêm antes de vértices de INICIO
    TipoVertice tipo1 = getVerticeTipo(*vert1);
    TipoVertice tipo2 = getVerticeTipo(*vert2);
    
    if (tipo1 == TIPO_FIM && tipo2 == TIPO_INICIO) return -1;
    if (tipo1 == TIPO_INICIO && tipo2 == TIPO_FIM) return 1;
    
    return 0;
}

void imprimeVertice(const Vertice v, FILE* arquivo) {
    if (v == NULL || arquivo == NULL) {
        return;
    }
    
    VerticeInterno* vert = (VerticeInterno*)v;
    
    fprintf(arquivo, "Vértice: (%.2f, %.2f) - Tipo: %s - Ângulo: %.4f rad (%.2f°)\n",
            vert->x, vert->y,
            vert->tipo == TIPO_INICIO ? "INICIO" : "FIM",
            vert->angulo,
            vert->angulo * 180.0 / PI);
}