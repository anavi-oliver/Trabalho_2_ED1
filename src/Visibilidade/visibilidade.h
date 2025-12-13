#ifndef VISIBILIDADE_H
#define VISIBILIDADE_H

#include <stdio.h>
#include "lista.h"

// Define PontoVis como um tipo opaco (ponteiro genérico)
// A estrutura real fica escondida no .c
typedef void* PontoVis;

/*
 * Calcula o poligono de visibilidade.
 * Retorna uma Lista de PontoVis (opacos).
 */
Lista calcular_visibilidade(double bx, double by, Lista formas, char tipo_sort, int threshold);

/*
 * Gera o SVG do poligono de visibilidade
 */
void desenhar_poligono_visibilidade(FILE* svg, Lista poligono, char* cor_preenchimento);

/*
 * Libera memória da lista de pontos
 */
void destruir_lista_pontos(Lista poligono);

//  FUNÇÕES GETTERS 
double getPontoVisX(PontoVis p);
double getPontoVisY(PontoVis p);

#endif