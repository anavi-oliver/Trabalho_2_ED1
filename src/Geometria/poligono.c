#include "poligono.h"
#include "geometria.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct poligono {
    Lista vertices;      
    Lista segmentos;     
    BoundingBox bbox;    
} PoligonoStruct;

/*                    FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO                    */

Poligono criaPoligono() {
    PoligonoStruct *p = (PoligonoStruct*) malloc(sizeof(PoligonoStruct));
    if (p == NULL) {
        fprintf(stderr, "Erro: falha na alocação de memória para polígono.\n");
        return NULL;
    }
    
    p->vertices = criaLista();
    p->segmentos = criaLista();
    p->bbox = criaBoundingBoxVazia();
    
    if (p->vertices == NULL || p->segmentos == NULL || p->bbox == NULL) {
        destroiLista(p->vertices);
        destroiLista(p->segmentos);
        destroiBoundingBox(p->bbox);
        free(p);
        return NULL;
    }
    
    return (Poligono) p;
}

void destroiPoligono(Poligono p) {
    if (p == NULL) {
        return;
    }
    
    PoligonoStruct *poli = (PoligonoStruct*) p;
    
    destroiListaCompleta(poli->vertices, (void (*)(void*))destroiPonto);
    destroiListaCompleta(poli->segmentos, (void (*)(void*))destroiSegmento);
    destroiBoundingBox(poli->bbox);
    
    free(poli);
}

/*                    OPERAÇÕES DE INSERÇÃO                    */

void insertVertice(Poligono p, Ponto v) {
    if (p == NULL || v == NULL) {
        return;
    }
    
    PoligonoStruct *poli = (PoligonoStruct*) p;
    
    Ponto copia = copiaPonto(v);
    if (copia == NULL) {
        return;
    }
    
    insereListaFim(poli->vertices, copia);
    
    expandeBBComPonto(poli->bbox, copia);
}

void insertSegmento(Poligono p, Segmento s) {
    if (p == NULL || s == NULL) {
        return;
    }
    
    PoligonoStruct *poli = (PoligonoStruct*) p;
    
    Ponto p1 = getPonto1Segmento(s);
    Ponto p2 = getPonto2Segmento(s);
    Segmento copia = criaSegmento(p1, p2);
    
    if (copia == NULL) {
        return;
    }
    
    insereListaFim(poli->segmentos, copia);
}

/*                    OPERAÇÕES DE CONSULTA                    */

bool pontoNoPoligono(Poligono p, Ponto pt) {
    if (p == NULL || pt == NULL) {
        return false;
    }
    
    PoligonoStruct *poli = (PoligonoStruct*) p;
    
    if (!pontoNaBB(poli->bbox, pt)) {
        return false;
    }
    
    int numVertices = tamanhoLista(poli->vertices);
    if (numVertices < 3) {
        return false;
    }
    
    double px = getXPonto(pt);
    double py = getYPonto(pt);
    
    int cruzamentos = 0;
    int i;
    
    for (i = 0; i < numVertices; i++) {
        Ponto v1 = (Ponto) getListaPosicao(poli->vertices, i);
        Ponto v2 = (Ponto) getListaPosicao(poli->vertices, (i + 1) % numVertices);
        
        double x1 = getXPonto(v1);
        double y1 = getYPonto(v1);
        double x2 = getXPonto(v2);
        double y2 = getYPonto(v2);
        
        if ((y1 > py) != (y2 > py)) {
            double xIntersecao = (x2 - x1) * (py - y1) / (y2 - y1) + x1;
            
            if (px < xIntersecao) {
                cruzamentos++;
            }
        }
    }
    
    return (cruzamentos % 2) == 1;
}

Lista getVertices(Poligono p) {
    if (p == NULL) {
        return NULL;
    }
    
    PoligonoStruct *poli = (PoligonoStruct*) p;
    return poli->vertices;
}

Lista getSegmentos(Poligono p) {
    if (p == NULL) {
        return NULL;
    }
    
    PoligonoStruct *poli = (PoligonoStruct*) p;
    return poli->segmentos;
}

BoundingBox getBoundingBox(Poligono p) {
    if (p == NULL) {
        return NULL;
    }
    
    PoligonoStruct *poli = (PoligonoStruct*) p;
    return poli->bbox;
}

int numeroVertices(Poligono p) {
    if (p == NULL) {
        return 0;
    }
    
    PoligonoStruct *poli = (PoligonoStruct*) p;
    return tamanhoLista(poli->vertices);
}

int numeroSegmentos(Poligono p) {
    if (p == NULL) {
        return 0;
    }
    
    PoligonoStruct *poli = (PoligonoStruct*) p;
    return tamanhoLista(poli->segmentos);
}

/*                    OPERAÇÕES GEOMÉTRICAS                    */

double perimetroPoligono(Poligono p) {
    if (p == NULL) {
        return 0.0;
    }
    
    PoligonoStruct *poli = (PoligonoStruct*) p;
    double perimetro = 0.0;
    
    int numSegmentos = tamanhoLista(poli->segmentos);
    int i;
    
    for (i = 0; i < numSegmentos; i++) {
        Segmento s = (Segmento) getListaPosicao(poli->segmentos, i);
        perimetro += comprimentoSegmento(s);
    }
    
    return perimetro;
}

double areaPoligono(Poligono p) {
    if (p == NULL) {
        return 0.0;
    }
    
    PoligonoStruct *poli = (PoligonoStruct*) p;
    int numVertices = tamanhoLista(poli->vertices);
    
    if (numVertices < 3) {
        return 0.0;
    }
    
    double area = 0.0;
    int i;
    
    for (i = 0; i < numVertices; i++) {
        Ponto p1 = (Ponto) getListaPosicao(poli->vertices, i);
        Ponto p2 = (Ponto) getListaPosicao(poli->vertices, (i + 1) % numVertices);
        
        double x1 = getXPonto(p1);
        double y1 = getYPonto(p1);
        double x2 = getXPonto(p2);
        double y2 = getYPonto(p2);
        
        area += (x1 * y2 - x2 * y1);
    }
    
    return fabs(area) / 2.0;
}

bool poligonoConvexo(Poligono p) {
    if (p == NULL) {
        return false;
    }
    
    PoligonoStruct *poli = (PoligonoStruct*) p;
    int numVertices = tamanhoLista(poli->vertices);
    
    if (numVertices < 3) {
        return false;
    }
    
    int sinal = 0;
    int i;
    
    for (i = 0; i < numVertices; i++) {
        Ponto p1 = (Ponto) getListaPosicao(poli->vertices, i);
        Ponto p2 = (Ponto) getListaPosicao(poli->vertices, (i + 1) % numVertices);
        Ponto p3 = (Ponto) getListaPosicao(poli->vertices, (i + 2) % numVertices);
        
        int dir = direcaoOrientacao(p1, p2, p3);
        
        if (dir != 0) {
            if (sinal == 0) {
                sinal = dir;
            } else if (sinal != dir) {
                return false;
            }
        }
    }
    
    return true;
}

/*                    CONSTRUÇÃO AUTOMÁTICA                    */

void fechaPoligono(Poligono p) {
    if (p == NULL) {
        return;
    }
    
    PoligonoStruct *poli = (PoligonoStruct*) p;
    int numVertices = tamanhoLista(poli->vertices);
    
    if (numVertices < 2) {
        return;
    }
    
    destroiListaCompleta(poli->segmentos, (void (*)(void*))destroiSegmento);
    poli->segmentos = criaLista();
    
    int i;
    for (i = 0; i < numVertices; i++) {
        Ponto p1 = (Ponto) getListaPosicao(poli->vertices, i);
        Ponto p2 = (Ponto) getListaPosicao(poli->vertices, (i + 1) % numVertices);
        
        Segmento s = criaSegmento(p1, p2);
        if (s != NULL) {
            insereListaFim(poli->segmentos, s);
        }
    }
}