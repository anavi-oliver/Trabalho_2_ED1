#include "ponto.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct ponto {
    double x;
    double y;
} PontoStruct;

/*                    FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO                    */

Ponto criaPonto(double x, double y) {
    PontoStruct *p = (PontoStruct*) malloc(sizeof(PontoStruct));
    if (p == NULL) {
        fprintf(stderr, "Erro: falha na alocação de memória para ponto.\n");
        return NULL;
    }
    
    p->x = x;
    p->y = y;
    
    return (Ponto) p;
}

void destroiPonto(Ponto p) {
    if (p == NULL) {
        return;
    }
    free(p);
}

/*                    MÉTODOS GET (CONSULTA)                    */

double getXPonto(Ponto p) {
    if (p == NULL) {
        return 0.0;
    }
    PontoStruct *pt = (PontoStruct*) p;
    return pt->x;
}

double getYPonto(Ponto p) {
    if (p == NULL) {
        return 0.0;
    }
    PontoStruct *pt = (PontoStruct*) p;
    return pt->y;
}

/*                    MÉTODOS SET (MODIFICAÇÃO)                    */

void setXPonto(Ponto p, double x) {
    if (p == NULL) {
        return;
    }
    PontoStruct *pt = (PontoStruct*) p;
    pt->x = x;
}

void setYPonto(Ponto p, double y) {
    if (p == NULL) {
        return;
    }
    PontoStruct *pt = (PontoStruct*) p;
    pt->y = y;
}

/*                    FUNÇÕES GEOMÉTRICAS                    */

double distanciaPontos(Ponto p1, Ponto p2) {
    if (p1 == NULL || p2 == NULL) {
        return 0.0;
    }
    
    PontoStruct *pt1 = (PontoStruct*) p1;
    PontoStruct *pt2 = (PontoStruct*) p2;
    
    double dx = pt2->x - pt1->x;
    double dy = pt2->y - pt1->y;
    
    return sqrt(dx * dx + dy * dy);
}

bool pontosIguais(Ponto p1, Ponto p2) {
    if (p1 == NULL || p2 == NULL) {
        return false;
    }
    
    PontoStruct *pt1 = (PontoStruct*) p1;
    PontoStruct *pt2 = (PontoStruct*) p2;
    
    double epsilon = 1e-9;
    return (fabs(pt1->x - pt2->x) < epsilon) && 
           (fabs(pt1->y - pt2->y) < epsilon);
}

Ponto copiaPonto(Ponto p) {
    if (p == NULL) {
        return NULL;
    }
    
    PontoStruct *pt = (PontoStruct*) p;
    return criaPonto(pt->x, pt->y);
}