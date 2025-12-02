#include "visibilidade.h"
#include "vertice.h"
#include "segsativos.h"
#include "ordenacao.h"
#include "segmento.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*________________________________ FUNÇÕES AUXILIARES ________________________________*/

double calculaAnguloPolar(double px, double py, double x, double y) {
    double dx = x - px;
    double dy = y - py;
    
    double angulo = atan2(dy, dx);
    
    // Normaliza para [0, 2π)
    if (angulo < 0) {
        angulo += 2.0 * M_PI;
    }
    
    return angulo;
}

double distanciaRaioSegmento(double px, double py, double angulo, Segmento segmento) {
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
    
    // Calcula interseção usando determinantes
    double det = dx_raio * dy_seg - dy_raio * dx_seg;
    
    if (fabs(det) < 1e-10) {
        return INFINITY;  // Paralelos
    }
    
    double t = (dx_p1 * dy_seg - dy_p1 * dx_seg) / det;
    double u = (dx_p1 * dy_raio - dy_p1 * dx_raio) / det;
    
    // Verifica se interseção está no raio (t >= 0) e no segmento (0 <= u <= 1)
    if (t >= 0 && u >= 0 && u <= 1) {
        return t;
    }
    
    return INFINITY;
}

bool segmentoEhVisivel(double px, double py, Segmento segmento,
                      Segmento* segmentosBloqueadores, int n) {
    if (segmento == NULL) {
        return false;
    }
    
    // Calcula ângulo do ponto médio do segmento
Ponto p1 = getPonto1Segmento(segmento);
Ponto p2 = getPonto2Segmento(segmento);
double x1 = getXPonto(p1);
double y1 = getYPonto(p1);
double x2 = getXPonto(p2);
double y2 = getYPonto(p2);
    
    double xm = (x1 + x2) / 2.0;
    double ym = (y1 + y2) / 2.0;
    
    double angulo = calculaAnguloPolar(px, py, xm, ym);
    double distSegmento = distanciaRaioSegmento(px, py, angulo, segmento);
    
    if (distSegmento == INFINITY) {
        return false;  // Segmento não intersecta raio
    }
    
    // Verifica se algum outro segmento está mais próximo
    for (int i = 0; i < n; i++) {
        if (segmentosBloqueadores[i] == segmento) {
            continue;  // Ignora o próprio segmento
        }
        
        double distBloqueador = distanciaRaioSegmento(px, py, angulo, segmentosBloqueadores[i]);
        
        if (distBloqueador < distSegmento - 1e-9) {
            return false;  // Bloqueado por outro segmento
        }
    }
    
    return true;
}


/*________________________________ ALGORITMO PRINCIPAL ________________________________*/

void calculaVisibilidade(double px, double py, 
                        Segmento* segmentos, int n,
                        bool* segmentosVisiveis) {
    if (segmentos == NULL || segmentosVisiveis == NULL || n <= 0) {
        return;
    }
    
    // Inicializa array de visibilidade
    for (int i = 0; i < n; i++) {
        segmentosVisiveis[i] = false;
    }
    
    // PASSO 1: Cria array de vértices (2 por segmento: INICIO e FIM)
    int numVertices = n * 2;
    Vertice* vertices = (Vertice*)malloc(numVertices * sizeof(Vertice));
    if (vertices == NULL) {
        fprintf(stderr, "ERRO: Falha ao alocar memória para vértices.\n");
        return;
    }
    
    // Calcula ângulos e cria vértices
    for (int i = 0; i < n; i++) {
        Ponto p1 = getPonto1Segmento(segmentos[i]);
        Ponto p2 = getPonto2Segmento(segmentos[i]);
        
        double x1 = getXPonto(p1);
        double y1 = getYPonto(p1);
        double x2 = getXPonto(p2);
        double y2 = getYPonto(p2);
        
        double angulo1 = calculaAnguloPolar(px, py, x1, y1);
        double angulo2 = calculaAnguloPolar(px, py, x2, y2);
        
        // Vértice de INICIO (menor ângulo)
        if (angulo1 <= angulo2) {
            vertices[2*i] = criaVertice(x1, y1, TIPO_INICIO, angulo1, segmentos[i]);
            vertices[2*i + 1] = criaVertice(x2, y2, TIPO_FIM, angulo2, segmentos[i]);
        } else {
            vertices[2*i] = criaVertice(x2, y2, TIPO_INICIO, angulo2, segmentos[i]);
            vertices[2*i + 1] = criaVertice(x1, y1, TIPO_FIM, angulo1, segmentos[i]);
        }
    }
    
    // PASSO 2: Ordena vértices por ângulo
ordena(vertices, numVertices, sizeof(Vertice), comparaVerticesPorAngulo, TIPO_QSORT, 0);
    
    // PASSO 3: Cria árvore de segmentos ativos
    SegmentosAtivos ativos = criaSegmentosAtivos(px, py);
    if (ativos == NULL) {
        for (int i = 0; i < numVertices; i++) {
            destroiVertice(vertices[i]);
        }
        free(vertices);
        return;
    }
    
    // PASSO 4: Varredura angular
    for (int i = 0; i < numVertices; i++) {
        Vertice v = vertices[i];
        double angulo = getVerticeAngulo(v);
        TipoVertice tipo = getVerticeTipo(v);
        Segmento segAtual = (Segmento)getVerticeSegmento(v);
        
        if (tipo == TIPO_INICIO) {
            // Insere segmento na árvore
            insereSegmentoAtivo(ativos, segAtual, angulo);
            
            // Verifica se este segmento ficou visível
            Segmento segMaisProximo = (Segmento)getSegmentoMaisProximo(ativos, angulo);
            
            if (segMaisProximo == segAtual) {
                // Este segmento é o mais próximo, logo é visível
                for (int j = 0; j < n; j++) {
                    if (segmentos[j] == segAtual) {
                        segmentosVisiveis[j] = true;
                        break;
                    }
                }
            }
        } else {  // TIPO_FIM
            // Remove segmento da árvore
            removeSegmentoAtivo(ativos, segAtual);
        }
    }
    
    // Libera memória
    destroiSegmentosAtivos(ativos);
    for (int i = 0; i < numVertices; i++) {
        destroiVertice(vertices[i]);
    }
    free(vertices);
}


/*________________________________ ESTRUTURAS DE DADOS ________________________________*/

typedef struct {
    double px;
    double py;
    int numVerticesVisiveis;
    double* angulosVisiveis;
    double* distanciasVisiveis;
} PoligonoVisibilidadeInterno;

PoligonoVisibilidade criaPoligonoVisibilidade(double px, double py) {
    PoligonoVisibilidadeInterno* pv = (PoligonoVisibilidadeInterno*)malloc(sizeof(PoligonoVisibilidadeInterno));
    if (pv == NULL) {
        return NULL;
    }
    
    pv->px = px;
    pv->py = py;
    pv->numVerticesVisiveis = 0;
    pv->angulosVisiveis = NULL;
    pv->distanciasVisiveis = NULL;
    
    return (PoligonoVisibilidade)pv;
}

void destroiPoligonoVisibilidade(PoligonoVisibilidade pv) {
    if (pv == NULL) {
        return;
    }
    
    PoligonoVisibilidadeInterno* poligono = (PoligonoVisibilidadeInterno*)pv;
    
    if (poligono->angulosVisiveis != NULL) {
        free(poligono->angulosVisiveis);
    }
    if (poligono->distanciasVisiveis != NULL) {
        free(poligono->distanciasVisiveis);
    }
    
    free(poligono);
}


/*________________________________ ANÁLISE E RELATÓRIOS ________________________________*/

int contaSegmentosVisiveis(const bool* segmentosVisiveis, int n) {
    if (segmentosVisiveis == NULL || n <= 0) {
        return 0;
    }
    
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (segmentosVisiveis[i]) {
            count++;
        }
    }
    
    return count;
}

void imprimeRelatorioVisibilidade(FILE* arquivo,
                                 Segmento* segmentos,
                                 bool* segmentosVisiveis,
                                 int n,
                                 double px, double py) {
    if (arquivo == NULL || segmentos == NULL || segmentosVisiveis == NULL || n <= 0) {
        return;
    }
    
    fprintf(arquivo, "\n========== RELATÓRIO DE VISIBILIDADE ==========\n");
    fprintf(arquivo, "Ponto de observação: (%.2f, %.2f)\n", px, py);
    fprintf(arquivo, "Número total de segmentos: %d\n", n);
    
    int numVisiveis = contaSegmentosVisiveis(segmentosVisiveis, n);
    fprintf(arquivo, "Segmentos visíveis: %d\n", numVisiveis);
    fprintf(arquivo, "Segmentos ocultos: %d\n", n - numVisiveis);
    fprintf(arquivo, "Percentual de visibilidade: %.1f%%\n", 
            (numVisiveis * 100.0) / n);
    
    fprintf(arquivo, "\n--- Segmentos Visíveis ---\n");
    for (int i = 0; i < n; i++) {
        if (segmentosVisiveis[i]) {
Ponto p1 = getPonto1Segmento(segmentos[i]);
Ponto p2 = getPonto2Segmento(segmentos[i]);
fprintf(arquivo, "  Segmento %d: (%.2f, %.2f) -> (%.2f, %.2f)\n", i,
        getXPonto(p1), getYPonto(p1),
        getXPonto(p2), getYPonto(p2));
        }
    }
    
    fprintf(arquivo, "\n--- Segmentos Ocultos ---\n");
    for (int i = 0; i < n; i++) {
        if (!segmentosVisiveis[i]) {
Ponto p1 = getPonto1Segmento(segmentos[i]);
Ponto p2 = getPonto2Segmento(segmentos[i]);
fprintf(arquivo, "  Segmento %d: (%.2f, %.2f) -> (%.2f, %.2f)\n",
        i,
        getXPonto(p1), getYPonto(p1),
        getXPonto(p2), getYPonto(p2));
        }
    }
    
    fprintf(arquivo, "===============================================\n\n");
}