#ifndef VISIBILIDADE_H
#define VISIBILIDADE_H

#include <stdbool.h>
#include <stdio.h>
#include "segmento.h"

/*
 * MÓDULO DE CÁLCULO DE VISIBILIDADE
 * 
 * Implementa o algoritmo de varredura angular para determinar
 * quais segmentos de reta são visíveis a partir de um ponto de observação.
 * 
 * O algoritmo:
 * 1. Calcula ângulos polares de todos os vértices em relação ao ponto
 * 2. Ordena vértices por ângulo
 * 3. Faz varredura angular mantendo segmentos ativos em uma ABB
 * 4. Determina visibilidade de cada segmento
 */

typedef void* PoligonoVisibilidade;

/*________________________________ ESTRUTURAS DE DADOS ________________________________*/

/*
 * Cria um polígono de visibilidade vazio.
 * 
 * px, py: coordenadas do ponto de observação
 * 
 * Pré-condição: nenhuma
 * Pós-condição: retorna ponteiro para o polígono criado,
 *               ou NULL em caso de falha
 */
PoligonoVisibilidade criaPoligonoVisibilidade(double px, double py);

/*
 * Libera memória do polígono de visibilidade.
 * 
 * Pré-condição: pv deve ser válido ou NULL
 * Pós-condição: memória é liberada
 */
void destroiPoligonoVisibilidade(PoligonoVisibilidade pv);


/*________________________________ ALGORITMO PRINCIPAL ________________________________*/

/*
 * Calcula a visibilidade de um conjunto de segmentos a partir de um ponto.
 * 
 * ALGORITMO:
 * 1. Para cada segmento, calcula ângulos polares de suas extremidades
 * 2. Cria vértices (INICIO/FIM) e ordena por ângulo
 * 3. Inicializa árvore de segmentos ativos
 * 4. Para cada vértice em ordem angular:
 *    - Se INICIO: insere segmento na árvore
 *    - Se FIM: remove segmento da árvore
 *    - Verifica visibilidade do segmento mais próximo
 * 
 * px, py: coordenadas do ponto de observação
 * segmentos: array de ponteiros para segmentos
 * n: número de segmentos
 * segmentosVisiveis: array de saída (bool) indicando visibilidade
 * 
 * Pré-condição: segmentos deve ser um array válido com n elementos,
 *               segmentosVisiveis deve ter espaço para n elementos
 * Pós-condição: segmentosVisiveis[i] = true se segmento i é visível,
 *               false caso contrário
 */
void calculaVisibilidade(double px, double py, 
                        Segmento* segmentos, int n,
                        bool* segmentosVisiveis);


/*________________________________ FUNÇÕES AUXILIARES ________________________________*/

/*
 * Calcula o ângulo polar de um ponto em relação a outro.
 * 
 * px, py: ponto de referência (origem)
 * x, y: ponto para calcular o ângulo
 * 
 * Pós-condição: retorna ângulo em radianos no intervalo [0, 2π)
 */
double calculaAnguloPolar(double px, double py, double x, double y);

/*
 * Verifica se um segmento é visível a partir de um ponto.
 * Um segmento é visível se não está completamente bloqueado por outros segmentos.
 * 
 * px, py: ponto de observação
 * segmento: segmento a verificar
 * segmentosBloqueadores: array de segmentos que podem bloquear
 * n: número de segmentos bloqueadores
 * 
 * Pré-condição: todos os parâmetros devem ser válidos
 * Pós-condição: retorna true se visível, false caso contrário
 */
bool segmentoEhVisivel(double px, double py, Segmento segmento,
                      Segmento* segmentosBloqueadores, int n);

/*
 * Calcula a distância de um ponto a um segmento ao longo de um raio.
 * 
 * px, py: origem do raio
 * angulo: ângulo do raio em radianos
 * segmento: segmento para calcular interseção
 * 
 * Pré-condição: segmento deve ser válido
 * Pós-condição: retorna distância se há interseção, INFINITY caso contrário
 */
double distanciaRaioSegmento(double px, double py, double angulo, Segmento segmento);


/*________________________________ ANÁLISE E RELATÓRIOS ________________________________*/

/*
 * Imprime relatório de visibilidade.
 * 
 * arquivo: arquivo de saída
 * segmentos: array de segmentos analisados
 * segmentosVisiveis: array de visibilidade calculada
 * n: número de segmentos
 * px, py: ponto de observação
 * 
 * Pré-condição: todos os parâmetros devem ser válidos
 * Pós-condição: relatório é escrito no arquivo
 */
void imprimeRelatorioVisibilidade(FILE* arquivo,
                                 Segmento* segmentos,
                                 bool* segmentosVisiveis,
                                 int n,
                                 double px, double py);

/*
 * Conta o número de segmentos visíveis.
 * 
 * segmentosVisiveis: array de visibilidade
 * n: número de segmentos
 * 
 * Pré-condição: segmentosVisiveis deve ser válido com n elementos
 * Pós-condição: retorna número de segmentos visíveis
 */
int contaSegmentosVisiveis(const bool* segmentosVisiveis, int n);

#endif // VISIBILIDADE_H