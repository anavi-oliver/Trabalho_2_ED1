#ifndef GEOMETRIA_H
#define GEOMETRIA_H

#include <stdbool.h>
#include <stdio.h>
#include "ponto.h"
#include "segmento.h"

/*
*        MÓDULO DE GEOMETRIA COMPUTACIONAL
*
*        Este módulo fornece funções auxiliares para operações
*        geométricas fundamentais, incluindo testes de orientação,
*        interseção de segmentos e cálculos de distância.
*/

/*                    FUNÇÕES DE ORIENTAÇÃO                    */

/*
Calcula a orientação de três pontos ordenados (p, q, r).
Usa o produto vetorial (determinante) para determinar se os pontos 
formam uma curva no sentido horário, anti-horário ou são colineares.

Fórmula: orientação = (qy - py) * (rx - qx) - (qx - px) * (ry - qy)

* p: primeiro ponto
* q: segundo ponto (pivô)
* r: terceiro ponto

Pré-condição: p, q e r devem ser válidos
Pós-condição: retorna:
  - 0.0 se colineares (com tolerância de 1e-10)
  - valor > 0 se orientação anti-horária (esquerda)
  - valor < 0 se orientação horária (direita)
*/
double orientacao(Ponto p, Ponto q, Ponto r);

/*
Versão simplificada que retorna apenas a direção da orientação.

* p: primeiro ponto
* q: segundo ponto (pivô)
* r: terceiro ponto

Pré-condição: p, q e r devem ser válidos
Pós-condição: retorna:
  - 0 se colineares
  - 1 se anti-horário
  - -1 se horário
*/
int direcaoOrientacao(Ponto p, Ponto q, Ponto r);

/*                    FUNÇÕES DE INTERSEÇÃO                    */

/*
Verifica se dois segmentos se intersectam.
Usa o teste de orientação para determinar interseção.

Casos considerados:
  1. Interseção geral (segmentos se cruzam)
  2. Casos especiais (pontos colineares)

* s1: primeiro segmento
* s2: segundo segmento

Pré-condição: s1 e s2 devem ser válidos
Pós-condição: retorna true se os segmentos se intersectam
*/
bool segmentosIntersectam(Segmento s1, Segmento s2);

/*
Calcula o ponto de interseção entre dois segmentos.
Assume que os segmentos se intersectam (verificar antes com segmentosIntersectam).

* s1: primeiro segmento
* s2: segundo segmento

Pré-condição: s1 e s2 devem ser válidos e se intersectar
Pós-condição: retorna o ponto de interseção, ou NULL se não intersectam
              ou se são paralelos/colineares
*/
Ponto intersecaoSegmentos(Segmento s1, Segmento s2);

/*                    FUNÇÕES DE DISTÂNCIA                    */

/*
Calcula a distância de um ponto a um segmento.
Considera três casos:
  1. Projeção está antes do início do segmento
  2. Projeção está depois do fim do segmento
  3. Projeção está no segmento

* p: ponto de referência
* s: segmento

Pré-condição: p e s devem ser válidos
Pós-condição: retorna a menor distância do ponto ao segmento
*/
double distanciaPontoAoSegmento(Ponto p, Segmento s);

/*                    FUNÇÕES AUXILIARES                    */

/*
Verifica se um ponto q está no segmento pr.
Assume que p, q e r são colineares.

* p: primeira extremidade do segmento
* q: ponto a verificar
* r: segunda extremidade do segmento

Pré-condição: p, q e r devem ser válidos e colineares
Pós-condição: retorna true se q está entre p e r
*/
bool pontoNoSegmentoPQR(Ponto p, Ponto q, Ponto r);

/*
Calcula a área de um triângulo formado por três pontos.
Usa metade do valor absoluto do produto vetorial.

* p1: primeiro vértice
* p2: segundo vértice
* p3: terceiro vértice

Pré-condição: p1, p2 e p3 devem ser válidos
Pós-condição: retorna a área do triângulo
*/
double areaTriangulo(Ponto p1, Ponto p2, Ponto p3);

/*
Calcula o ângulo entre dois segmentos em radianos.
O ângulo é medido no sentido anti-horário de s1 para s2.

* s1: primeiro segmento
* s2: segundo segmento

Pré-condição: s1 e s2 devem ser válidos
Pós-condição: retorna o ângulo em radianos [0, 2π)
*/
double anguloEntreSegmentos(Segmento s1, Segmento s2);

#endif