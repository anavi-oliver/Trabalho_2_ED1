#ifndef SEGSATIVOS_H
#define SEGSATIVOS_H

#include <stdbool.h>
#include <stdio.h>

/*
 * TIPO ABSTRATO DE DADOS: ÁRVORE DE SEGMENTOS ATIVOS
 * 
 * Implementa uma árvore binária de busca (ABB) para gerenciar
 * segmentos de reta ativos durante a varredura angular no
 * algoritmo de visibilidade.
 * 
 * Os segmentos são ordenados pela distância ao ponto de observação
 * ao longo de um raio de varredura.
 */

typedef void* SegmentosAtivos;
typedef void* NoSegmento;

/*________________________________ FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO ________________________________*/

/*
 * Cria uma nova árvore de segmentos ativos vazia.
 * 
 * px, py: coordenadas do ponto de observação (usado para comparações)
 * 
 * Pré-condição: nenhuma
 * Pós-condição: retorna um ponteiro opaco para a árvore criada,
 *               ou NULL em caso de falha de alocação
 */
SegmentosAtivos criaSegmentosAtivos(double px, double py);

/*
 * Libera toda a memória alocada para a árvore de segmentos ativos.
 * Nota: NÃO libera os segmentos em si, apenas os nós da árvore.
 * 
 * sa: ponteiro para a árvore
 * 
 * Pré-condição: sa deve ser um ponteiro válido ou NULL
 * Pós-condição: memória da árvore é liberada
 */
void destroiSegmentosAtivos(SegmentosAtivos sa);


/*________________________________ OPERAÇÕES DE INSERÇÃO E REMOÇÃO ________________________________*/

/*
 * Insere um segmento na árvore de segmentos ativos.
 * O segmento é posicionado de acordo com sua distância ao ponto de observação
 * no ângulo atual de varredura.
 * 
 * sa: árvore de segmentos ativos
 * segmento: ponteiro para o segmento a ser inserido
 * anguloVarredura: ângulo atual da varredura (em radianos)
 * 
 * Pré-condição: sa e segmento devem ser ponteiros válidos
 * Pós-condição: segmento é inserido na árvore mantendo a ordem
 */
void insereSegmentoAtivo(SegmentosAtivos sa, void* segmento, double anguloVarredura);

/*
 * Remove um segmento da árvore de segmentos ativos.
 * 
 * sa: árvore de segmentos ativos
 * segmento: ponteiro para o segmento a ser removido
 * 
 * Pré-condição: sa e segmento devem ser ponteiros válidos
 * Pós-condição: segmento é removido da árvore (se encontrado)
 *               retorna true se removido, false caso contrário
 */
bool removeSegmentoAtivo(SegmentosAtivos sa, void* segmento);


/*________________________________ CONSULTAS ________________________________*/

/*
 * Retorna o segmento mais próximo do ponto de observação no ângulo atual.
 * Este é o segmento que bloqueia a visão no raio de varredura.
 * 
 * sa: árvore de segmentos ativos
 * anguloVarredura: ângulo atual da varredura (em radianos)
 * 
 * Pré-condição: sa deve ser um ponteiro válido
 * Pós-condição: retorna ponteiro para o segmento mais próximo,
 *               ou NULL se a árvore estiver vazia
 */
void* getSegmentoMaisProximo(const SegmentosAtivos sa, double anguloVarredura);

/*
 * Verifica se a árvore está vazia.
 * 
 * Pré-condição: sa deve ser um ponteiro válido
 * Pós-condição: retorna true se vazia, false caso contrário
 */
bool segmentosAtivosVazio(const SegmentosAtivos sa);

/*
 * Retorna o número de segmentos ativos na árvore.
 * 
 * Pré-condição: sa deve ser um ponteiro válido
 * Pós-condição: retorna o número de segmentos
 */
int getNumeroSegmentosAtivos(const SegmentosAtivos sa);


/*________________________________ FUNÇÕES AUXILIARES ________________________________*/

/*
 * Retorna as coordenadas do ponto de observação.
 * 
 * sa: árvore de segmentos ativos
 * px, py: ponteiros para armazenar as coordenadas (saída)
 * 
 * Pré-condição: sa, px e py devem ser ponteiros válidos
 * Pós-condição: coordenadas são armazenadas em px e py
 */
void getPontoObservacao(const SegmentosAtivos sa, double* px, double* py);

/*
 * Imprime a árvore de segmentos ativos (para debug).
 * 
 * sa: árvore a ser impressa
 * arquivo: arquivo de saída (pode ser stdout)
 * 
 * Pré-condição: sa e arquivo devem ser válidos
 * Pós-condição: estrutura da árvore é impressa no arquivo
 */
void imprimeSegmentosAtivos(const SegmentosAtivos sa, FILE* arquivo);

#endif // SEGSATIVOS_H