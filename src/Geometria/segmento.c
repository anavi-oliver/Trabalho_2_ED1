#include "segmento.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct segmento {
    Ponto p1;
    Ponto p2;
} SegmentoStruct;

/*                    FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO                    */

Segmento criaSegmento(Ponto p1, Ponto p2) {
    if (p1 == NULL || p2 == NULL) {
        fprintf(stderr, "Erro: pontos inválidos para criar segmento.\n");
        return NULL;
    }
    
    SegmentoStruct *s = (SegmentoStruct*) malloc(sizeof(SegmentoStruct));
    if (s == NULL) {
        fprintf(stderr, "Erro: falha na alocação de memória para segmento.\n");
        return NULL;
    }
    
    s->p1 = copiaPonto(p1);
    s->p2 = copiaPonto(p2);
    
    if (s->p1 == NULL || s->p2 == NULL) {
        destroiPonto(s->p1);
        destroiPonto(s->p2);
        free(s);
        return NULL;
    }
    
    return (Segmento) s;
}

Segmento criaSegmentoXY(double x1, double y1, double x2, double y2) {
    Ponto p1 = criaPonto(x1, y1);
    Ponto p2 = criaPonto(x2, y2);
    
    if (p1 == NULL || p2 == NULL) {
        destroiPonto(p1);
        destroiPonto(p2);
        return NULL;
    }
    
    Segmento s = criaSegmento(p1, p2);
    
    destroiPonto(p1);
    destroiPonto(p2);
    
    return s;
}

void destroiSegmento(Segmento s) {
    if (s == NULL) {
        return;
    }
    
    SegmentoStruct *seg = (SegmentoStruct*) s;
    destroiPonto(seg->p1);
    destroiPonto(seg->p2);
    free(seg);
}

/*                    MÉTODOS GET (CONSULTA)                    */

Ponto getPonto1Segmento(Segmento s) {
    if (s == NULL) {
        return NULL;
    }
    SegmentoStruct *seg = (SegmentoStruct*) s;
    return seg->p1;
}

Ponto getPonto2Segmento(Segmento s) {
    if (s == NULL) {
        return NULL;
    }
    SegmentoStruct *seg = (SegmentoStruct*) s;
    return seg->p2;
}

/*                    FUNÇÕES GEOMÉTRICAS                    */

double comprimentoSegmento(Segmento s) {
    if (s == NULL) {
        return 0.0;
    }
    
    SegmentoStruct *seg = (SegmentoStruct*) s;
    return distanciaPontos(seg->p1, seg->p2);
}

bool pontoNoSegmento(Segmento s, Ponto p) {
    if (s == NULL || p == NULL) {
        return false;
    }
    
    SegmentoStruct *seg = (SegmentoStruct*) s;
    
    double x = getXPonto(p);
    double y = getYPonto(p);
    double x1 = getXPonto(seg->p1);
    double y1 = getYPonto(seg->p1);
    double x2 = getXPonto(seg->p2);
    double y2 = getYPonto(seg->p2);
    
    double minX = (x1 < x2) ? x1 : x2;
    double maxX = (x1 > x2) ? x1 : x2;
    double minY = (y1 < y2) ? y1 : y2;
    double maxY = (y1 > y2) ? y1 : y2;
    
    return (x >= minX && x <= maxX && y >= minY && y <= maxY);
}

double distanciaPontoSegmento(Ponto p, Segmento s) {
    if (p == NULL || s == NULL) {
        return 0.0;
    }
    
    SegmentoStruct *seg = (SegmentoStruct*) s;
    
    double px = getXPonto(p);
    double py = getYPonto(p);
    double x1 = getXPonto(seg->p1);
    double y1 = getYPonto(seg->p1);
    double x2 = getXPonto(seg->p2);
    double y2 = getYPonto(seg->p2);
    
    double dx = x2 - x1;
    double dy = y2 - y1;
    
    double comprimentoQuad = dx * dx + dy * dy;
    
    if (comprimentoQuad == 0.0) {
        return distanciaPontos(p, seg->p1);
    }
    
    double t = ((px - x1) * dx + (py - y1) * dy) / comprimentoQuad;
    
    if (t < 0.0) {
        return distanciaPontos(p, seg->p1);
    }
    if (t > 1.0) {
        return distanciaPontos(p, seg->p2);
    }
    
    Ponto projecao = criaPonto(x1 + t * dx, y1 + t * dy);
    double dist = distanciaPontos(p, projecao);
    destroiPonto(projecao);
    
    return dist;
}