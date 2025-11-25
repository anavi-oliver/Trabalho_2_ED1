#ifndef VERTICE_H
#define VERTICE_H

#include <stdbool.h>
#include <stdio.h>

/*
 * TIPO ABSTRATO DE DADOS: VÉRTICE
 * 
 * Representa um vértice (ponto de extremidade) de um segmento de linha.
 * Cada vértice possui coordenadas (x,y), um tipo (INICIO ou FIM), 
 * um ângulo polar em relação a um ponto de referência, e uma referência 
 * ao segmento ao qual pertence.
 */

typedef enum {
    TIPO_INICIO,    // Vértice de início do segmento
    TIPO_FIM        // Vértice de fim do segmento
} TipoVertice;

typedef void* Vertice;

/*________________________________ FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO ________________________________*/

/*
 * Cria um novo vértice com as propriedades especificadas.
 * 
 * x, y: coordenadas do vértice no plano cartesiano
 * tipo: TIPO_INICIO ou TIPO_FIM
 * angulo: ângulo polar em relação ao ponto de observação (em radianos)
 * segmento: ponteiro para o segmento ao qual o vértice pertence
 * 
 * Pré-condição: segmento deve ser um ponteiro válido
 * Pós-condição: retorna um ponteiro opaco para o Vértice criado,
 *               ou NULL em caso de falha de alocação
 */
Vertice criaVertice(double x, double y, TipoVertice tipo, double angulo, void* segmento);

/*
 * Libera a memória alocada para o vértice.
 * 
 * v: ponteiro para o vértice a ser destruído
 * 
 * Pré-condição: v deve ser um ponteiro válido ou NULL
 * Pós-condição: memória do vértice é liberada
 */
void destroiVertice(Vertice v);


/*________________________________ MÉTODOS GET (CONSULTA) ________________________________*/

/*
 * Retorna a coordenada X do vértice.
 * 
 * Pré-condição: v deve ser um ponteiro válido
 * Pós-condição: retorna a coordenada X como double
 */
double getVerticeX(const Vertice v);

/*
 * Retorna a coordenada Y do vértice.
 * 
 * Pré-condição: v deve ser um ponteiro válido
 * Pós-condição: retorna a coordenada Y como double
 */
double getVerticeY(const Vertice v);

/*
 * Retorna o tipo do vértice (INICIO ou FIM).
 * 
 * Pré-condição: v deve ser um ponteiro válido
 * Pós-condição: retorna o tipo do vértice
 */
TipoVertice getVerticeTipo(const Vertice v);

/*
 * Retorna o ângulo polar do vértice em relação ao ponto de observação.
 * 
 * Pré-condição: v deve ser um ponteiro válido
 * Pós-condição: retorna o ângulo em radianos
 */
double getVerticeAngulo(const Vertice v);

/*
 * Retorna o ponteiro para o segmento ao qual o vértice pertence.
 * 
 * Pré-condição: v deve ser um ponteiro válido
 * Pós-condição: retorna ponteiro para o segmento
 */
void* getVerticeSegmento(const Vertice v);


/*________________________________ MÉTODOS SET (MODIFICAÇÃO) ________________________________*/

/*
 * Define a coordenada X do vértice.
 * 
 * Pré-condição: v deve ser um ponteiro válido
 * Pós-condição: coordenada X é atualizada
 */
void setVerticeX(Vertice v, double x);

/*
 * Define a coordenada Y do vértice.
 * 
 * Pré-condição: v deve ser um ponteiro válido
 * Pós-condição: coordenada Y é atualizada
 */
void setVerticeY(Vertice v, double y);

/*
 * Define o tipo do vértice.
 * 
 * Pré-condição: v deve ser um ponteiro válido
 * Pós-condição: tipo do vértice é atualizado
 */
void setVerticeTipo(Vertice v, TipoVertice tipo);

/*
 * Define o ângulo polar do vértice.
 * 
 * Pré-condição: v deve ser um ponteiro válido
 * Pós-condição: ângulo é atualizado
 */
void setVerticeAngulo(Vertice v, double angulo);

/*
 * Define o segmento ao qual o vértice pertence.
 * 
 * Pré-condição: v deve ser um ponteiro válido
 * Pós-condição: referência ao segmento é atualizada
 */
void setVerticeSegmento(Vertice v, void* segmento);


/*________________________________ FUNÇÕES UTILITÁRIAS ________________________________*/

/*
 * Compara dois vértices por ângulo (para ordenação).
 * Retorna: <0 se v1 < v2, 0 se v1 == v2, >0 se v1 > v2
 * 
 * Pré-condição: v1 e v2 devem ser ponteiros válidos
 * Pós-condição: retorna o resultado da comparação
 */
int comparaVerticesPorAngulo(const void* v1, const void* v2);

/*
 * Imprime informações do vértice para debug.
 * 
 * v: vértice a ser impresso
 * arquivo: arquivo de saída (pode ser stdout)
 * 
 * Pré-condição: v e arquivo devem ser válidos
 * Pós-condição: informações são escritas no arquivo
 */
void imprimeVertice(const Vertice v, FILE* arquivo);

#endif // VERTICE_H