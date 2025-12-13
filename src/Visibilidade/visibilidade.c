#include "visibilidade.h"
#include "sort.h"
#include "formas.h"
#include "lista.h"
#include "linha.h"
#include "retangulo.h" 
#include "circulo.h" 

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef HUGE_VAL
#define HUGE_VAL 1e50
#endif

// --- DEFINIÇÃO DA STRUCT OPACA ---
typedef struct stPontoVis {
    double x;
    double y;
} StPontoVis;

// --- IMPLEMENTAÇÃO DOS GETTERS ---
double getPontoVisX(PontoVis p) {
    return ((StPontoVis*)p)->x;
}

double getPontoVisY(PontoVis p) {
    return ((StPontoVis*)p)->y;
}

// --- ESTRUTURAS AUXILIARES INTERNAS ---
typedef struct {
    double x1, y1;
    double x2, y2;
} SegmentoVar;

#define EV_INICIO 0
#define EV_FIM 1

typedef struct {
    double angulo;
    int tipo; 
    double x, y; 
    SegmentoVar* seg; 
} Evento;

// --- GEOMETRIA BÁSICA ---

double calcular_angulo(double cy, double cx, double py, double px) {
    return atan2(py - cy, px - cx);
}

double interseccao_raio_seg(double ox, double oy, double angulo, SegmentoVar* seg) {
    double dx = cos(angulo);
    double dy = sin(angulo);
    
    double x3 = seg->x1; double y3 = seg->y1;
    double x4 = seg->x2; double y4 = seg->y2;
    
    double denom = (x3 - x4)*dy - (y3 - y4)*dx;
    if (fabs(denom) < 1e-9) return HUGE_VAL; 
    
    double t = ((x3 - ox)*(y3 - y4) - (y3 - oy)*(x3 - x4)) / denom;
    double u = ((x3 - ox)*dy - (y3 - oy)*dx) / denom;
    
    if (t > 1e-6 && u >= 0.0 && u <= 1.0) {
        return t;
    }
    return HUGE_VAL;
}

int comparar_eventos(const void* a, const void* b) {
    Evento* e1 = *(Evento**)a;
    Evento* e2 = *(Evento**)b;

    if (e1->angulo < e2->angulo) return -1;
    if (e1->angulo > e2->angulo) return 1;

    if (e1->tipo == EV_INICIO && e2->tipo == EV_FIM) return -1;
    if (e1->tipo == EV_FIM && e2->tipo == EV_INICIO) return 1;

    return 0;
}

void add_seg(SegmentoVar** arr, int* cap, int* count, double x1, double y1, double x2, double y2) {
    if (*count >= *cap) {
        *cap *= 2;
        *arr = realloc(*arr, (*cap) * sizeof(SegmentoVar));
    }
    (*arr)[*count].x1 = x1;
    (*arr)[*count].y1 = y1;
    (*arr)[*count].x2 = x2;
    (*arr)[*count].y2 = y2;
    (*count)++;
}
int extrair_segmentos(double bx, double by, Lista formas, SegmentoVar** array_segs) {
    int capacidade = 100;
    int count = 0;
    *array_segs = malloc(capacidade * sizeof(SegmentoVar));
    (void)bx; (void)by; 

    if (!formas) return 0; // Proteção contra lista nula

    int qtd = tamanhoLista(formas);
    for (int i = 0; i < qtd; i++) {
        Forma f = (Forma) getListaPosicao(formas, i);
        
        // --- PROTEÇÃO EXTRA ---
        if (!f) continue; 
        void* obj = getFormaAssoc(f);
        if (!obj) continue; // Se a forma não tem dados associados, pula
        // ----------------------

        TipoForma tipo = getFormaTipo(f);

        if (tipo == TIPO_LINHA) {
            add_seg(array_segs, &capacidade, &count, getX1Linha(obj), getY1Linha(obj), getX2Linha(obj), getY2Linha(obj));
        }
        else if (tipo == TIPO_RETANGULO) {
            double x = getXRetangulo(obj);
            double y = getYRetangulo(obj);
            double w = getLarguraRetangulo(obj);
            double h = getAlturaRetangulo(obj);
            
            add_seg(array_segs, &capacidade, &count, x, y, x+w, y);     
            add_seg(array_segs, &capacidade, &count, x+w, y, x+w, y+h); 
            add_seg(array_segs, &capacidade, &count, x+w, y+h, x, y+h); 
            add_seg(array_segs, &capacidade, &count, x, y+h, x, y);     
        }
    }
    return count;
}

Lista calcular_visibilidade(double bx, double by, Lista formas, char tipo_sort, int threshold) {
    SegmentoVar* segs = NULL;
    int qtd_segs = extrair_segmentos(bx, by, formas, &segs);

    if (qtd_segs == 0) {
        free(segs);
        return NULL; // Ou retorna lista vazia
    }

    int qtd_ev = qtd_segs * 2;
    Evento** eventos = malloc(qtd_ev * sizeof(Evento*));
    
    for (int i = 0; i < qtd_segs; i++) {
        double ang1 = calcular_angulo(by, bx, segs[i].y1, segs[i].x1);
        double ang2 = calcular_angulo(by, bx, segs[i].y2, segs[i].x2);
        
        eventos[2*i] = malloc(sizeof(Evento));
        eventos[2*i]->angulo = ang1;
        eventos[2*i]->x = segs[i].x1;
        eventos[2*i]->y = segs[i].y1;
        eventos[2*i]->tipo = EV_INICIO; 
        eventos[2*i]->seg = &segs[i];

        eventos[2*i+1] = malloc(sizeof(Evento));
        eventos[2*i+1]->angulo = ang2;
        eventos[2*i+1]->x = segs[i].x2;
        eventos[2*i+1]->y = segs[i].y2;
        eventos[2*i+1]->tipo = EV_FIM;
        eventos[2*i+1]->seg = &segs[i];
    }

    if (tipo_sort == 'm') {
        merge_sort((void**)eventos, 0, qtd_ev - 1, threshold, comparar_eventos);
    } else {
        qsort(eventos, qtd_ev, sizeof(Evento*), (int (*)(const void*, const void*))comparar_eventos);
    }

    Lista poligono = criaLista(); 
    
    for (int i = 0; i < qtd_ev; i++) {
        double ang = eventos[i]->angulo;
        double menorT = HUGE_VAL;
        
        for (int k = 0; k < qtd_segs; k++) {
            double t = interseccao_raio_seg(bx, by, ang, &segs[k]);
            if (t < menorT) menorT = t;
        }
        
        if (menorT < HUGE_VAL) {
            // ALOCA A STRUCT INTERNA
            StPontoVis* p = malloc(sizeof(StPontoVis));
            p->x = bx + menorT * cos(ang);
            p->y = by + menorT * sin(ang);
            
            // Insere na lista (cast implicito para void*)
            insereListaFim(poligono, p);
        }
        free(eventos[i]);
    }
    
    free(eventos);
    free(segs);
    return poligono;
}

void desenhar_poligono_visibilidade(FILE* svg, Lista poligono, char* cor) {
    if (!poligono || !svg) return;
    
    fprintf(svg, "\n<polygon points=\"");
    
    int qtd = tamanhoLista(poligono);
    for(int i=0; i<qtd; i++) {
        PontoVis p = getListaPosicao(poligono, i);
        // USA OS GETTERS
        fprintf(svg, "%.2f,%.2f ", getPontoVisX(p), getPontoVisY(p));
    }
    
    fprintf(svg, "\" fill=\"%s\" opacity=\"0.5\" stroke=\"none\" />\n", cor);
}

void destruir_lista_pontos(Lista poligono) {
    destroiListaCompleta(poligono, free);
}