#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <stdbool.h>
#include "ponto.h"

/*
*        TIPO ABSTRATO DE DADOS: BOUNDING BOX
*
*        Este módulo define uma caixa delimitadora (bounding box) alinhada
*        aos eixos (AABB - Axis-Aligned Bounding Box). Útil para otimizar
*        testes de interseção através de testes rápidos de sobreposição.
*/

typedef void *BoundingBox;

/*                    FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO                    */

/*
Cria uma bounding box a partir de coordenadas mínimas e máximas.

* xMin: coordenada X mínima
* yMin: coordenada Y mínima
* xMax: coordenada X máxima
* yMax: coordenada Y máxima

Pré-condição: xMin <= xMax e yMin <= yMax
Pós-condição: retorna um ponteiro opaco para a bounding box criada,
              ou NULL em caso de falha
*/
BoundingBox criaBoundingBox(double xMin, double yMin, double xMax, double yMax);

/*
Cria uma bounding box vazia (infinito negativo para máximos, 
infinito positivo para mínimos). Útil para inicializar antes de expandir.

Pré-condição: nenhuma
Pós-condição: retorna uma bounding box vazia
*/
BoundingBox criaBoundingBoxVazia();

/*
Libera a memória alocada para uma bounding box.

* bb: ponteiro para a bounding box

Pré-condição: bb deve ser válido ou NULL
Pós-condição: memória da bounding box é liberada
*/
void destroiBoundingBox(BoundingBox bb);

/*                    MÉTODOS GET (CONSULTA)                    */

/*
Retorna a coordenada X mínima da bounding box.

* bb: ponteiro para a bounding box

Pré-condição: bb deve ser válido
Pós-condição: retorna xMin
*/
double getXMinBB(BoundingBox bb);

/*
Retorna a coordenada Y mínima da bounding box.

* bb: ponteiro para a bounding box

Pré-condição: bb deve ser válido
Pós-condição: retorna yMin
*/
double getYMinBB(BoundingBox bb);

/*
Retorna a coordenada X máxima da bounding box.

* bb: ponteiro para a bounding box

Pré-condição: bb deve ser válido
Pós-condição: retorna xMax
*/
double getXMaxBB(BoundingBox bb);

/*
Retorna a coordenada Y máxima da bounding box.

* bb: ponteiro para a bounding box

Pré-condição: bb deve ser válido
Pós-condição: retorna yMax
*/
double getYMaxBB(BoundingBox bb);

/*                    OPERAÇÕES GEOMÉTRICAS                    */

/*
Expande a bounding box para incluir um ponto.

* bb: ponteiro para a bounding box
* p: ponto a ser incluído

Pré-condição: bb e p devem ser válidos
Pós-condição: bounding box é expandida se necessário
*/
void expandeBBComPonto(BoundingBox bb, Ponto p);

/*
Expande a bounding box para incluir outra bounding box.

* bb: ponteiro para a bounding box a ser expandida
* outra: bounding box a ser incluída

Pré-condição: bb e outra devem ser válidos
Pós-condição: bb é expandida para conter outra
*/
void expandeBBComBB(BoundingBox bb, BoundingBox outra);

/*
Verifica se um ponto está dentro da bounding box.

* bb: ponteiro para a bounding box
* p: ponto a verificar

Pré-condição: bb e p devem ser válidos
Pós-condição: retorna true se o ponto está dentro (inclusive nas bordas)
*/
bool pontoNaBB(BoundingBox bb, Ponto p);

/*
Verifica se duas bounding boxes se intersectam.

* bb1: primeira bounding box
* bb2: segunda bounding box

Pré-condição: ambas devem ser válidas
Pós-condição: retorna true se há interseção
*/
bool BBsIntersectam(BoundingBox bb1, BoundingBox bb2);

/*
Calcula a área da bounding box.

* bb: ponteiro para a bounding box

Pré-condição: bb deve ser válido
Pós-condição: retorna a área (largura * altura)
*/
double areaBB(BoundingBox bb);

/*
Calcula a largura da bounding box.

* bb: ponteiro para a bounding box

Pré-condição: bb deve ser válido
Pós-condição: retorna a largura (xMax - xMin)
*/
double larguraBB(BoundingBox bb);

/*
Calcula a altura da bounding box.

* bb: ponteiro para a bounding box

Pré-condição: bb deve ser válido
Pós-condição: retorna a altura (yMax - yMin)
*/
double alturaBB(BoundingBox bb);

#endif