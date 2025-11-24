#ifndef SEGMENTO_H
#define SEGMENTO_H

#include <stdbool.h>
#include "ponto.h"

/*
*        TIPO ABSTRATO DE DADOS: SEGMENTO
*
*        Este módulo define a interface para manipulação de um segmento de reta
*        como Tipo Abstrato de Dados (TAD).  
*        Um segmento é definido por dois pontos (extremidades).
*/

typedef void *Segmento;

/*                    FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO                    */

/*
Cria um novo segmento de reta definido por dois pontos.
O segmento mantém cópias dos pontos fornecidos.

* p1: primeiro ponto (extremidade inicial)
* p2: segundo ponto (extremidade final)

Pré-condição: p1 e p2 devem ser válidos
Pós-condição: retorna um ponteiro opaco para o segmento criado,
              ou NULL em caso de falha
*/
Segmento criaSegmento(Ponto p1, Ponto p2);

/*
Cria um segmento a partir de coordenadas diretas.

* x1, y1: coordenadas do primeiro ponto
* x2, y2: coordenadas do segundo ponto

Pré-condição: nenhuma
Pós-condição: retorna um ponteiro opaco para o segmento criado,
              ou NULL em caso de falha
*/
Segmento criaSegmentoXY(double x1, double y1, double x2, double y2);

/*
Libera a memória alocada para um segmento.

* s: ponteiro para o segmento

Pré-condição: s deve ser válido ou NULL
Pós-condição: memória do segmento e seus pontos é liberada
*/
void destroiSegmento(Segmento s);

/*                    MÉTODOS GET (CONSULTA)                    */

/*
Retorna o primeiro ponto do segmento (extremidade inicial).

* s: ponteiro para o segmento

Pré-condição: s deve ser válido
Pós-condição: retorna o ponto inicial do segmento
*/
Ponto getPonto1Segmento(Segmento s);

/*
Retorna o segundo ponto do segmento (extremidade final).

* s: ponteiro para o segmento

Pré-condição: s deve ser válido
Pós-condição: retorna o ponto final do segmento
*/
Ponto getPonto2Segmento(Segmento s);

/*                    FUNÇÕES GEOMÉTRICAS                    */

/*
Calcula o comprimento do segmento.

* s: ponteiro para o segmento

Pré-condição: s deve ser válido
Pós-condição: retorna o comprimento do segmento
*/
double comprimentoSegmento(Segmento s);

/*
Verifica se um ponto está contido no segmento.
Assume que o ponto é colinear ao segmento.

* s: ponteiro para o segmento
* p: ponto a verificar

Pré-condição: s e p devem ser válidos
Pós-condição: retorna true se o ponto está no segmento
*/
bool pontoNoSegmento(Segmento s, Ponto p);

/*
Calcula a distância de um ponto a um segmento.

* s: ponteiro para o segmento
* p: ponto de referência

Pré-condição: s e p devem ser válidos
Pós-condição: retorna a menor distância do ponto ao segmento
*/
double distanciaPontoSegmento(Ponto p, Segmento s);

#endif