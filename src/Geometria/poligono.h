#ifndef POLIGONO_H
#define POLIGONO_H

#include <stdbool.h>
#include "lista.h"
#include "ponto.h"
#include "segmento.h"
#include "boundingbox.h"

/*
*        TIPO ABSTRATO DE DADOS: POLÍGONO
*
*        Este módulo define um polígono como uma sequência ordenada de vértices.
*        O polígono pode ser usado para representar regiões de visibilidade
*        das bombas no projeto.
*
*        Características:
*        - Mantém listas de vértices e segmentos (arestas)
*        - Calcula e mantém bounding box atualizada
*        - Suporta testes de ponto dentro do polígono
*/

typedef void *Poligono;

/*                    FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO                    */

/*
Cria um novo polígono vazio.

Pré-condição: nenhuma
Pós-condição: retorna um ponteiro opaco para o polígono criado,
              ou NULL em caso de falha
*/
Poligono criaPoligono();

/*
Libera toda a memória alocada pelo polígono, incluindo suas listas
de vértices e segmentos.

* p: ponteiro para o polígono

Pré-condição: p deve ser válido ou NULL
Pós-condição: memória do polígono e seus elementos é liberada
*/
void destroiPoligono(Poligono p);

/*                    OPERAÇÕES DE INSERÇÃO                    */

/*
Insere um vértice no polígono.
O vértice é adicionado ao final da lista de vértices.
Uma cópia do ponto é criada internamente.

* p: ponteiro para o polígono
* v: vértice (ponto) a ser inserido

Pré-condição: p e v devem ser válidos
Pós-condição: vértice é adicionado e bounding box é atualizada
*/
void insertVertice(Poligono p, Ponto v);

/*
Insere um segmento (aresta) no polígono.
Uma cópia do segmento é criada internamente.

* p: ponteiro para o polígono
* s: segmento a ser inserido

Pré-condição: p e s devem ser válidos
Pós-condição: segmento é adicionado ao polígono
*/
void insertSegmento(Poligono p, Segmento s);

/*                    OPERAÇÕES DE CONSULTA                    */

/*
Verifica se um ponto está dentro do polígono.
Usa o algoritmo Ray Casting (traça um raio horizontal do ponto
para o infinito e conta quantas vezes cruza as arestas).

Se o número de cruzamentos for ímpar, o ponto está dentro.
Se for par, está fora.

* p: ponteiro para o polígono
* pt: ponto a verificar

Pré-condição: p e pt devem ser válidos
Pós-condição: retorna true se o ponto está dentro do polígono
*/
bool pontoNoPoligono(Poligono p, Ponto pt);

/*
Retorna a lista de vértices do polígono.
A lista retornada é a lista interna (não uma cópia).

* p: ponteiro para o polígono

Pré-condição: p deve ser válido
Pós-condição: retorna a lista de vértices (Pontos)
*/
Lista getVertices(Poligono p);

/*
Retorna a lista de segmentos (arestas) do polígono.
A lista retornada é a lista interna (não uma cópia).

* p: ponteiro para o polígono

Pré-condição: p deve ser válido
Pós-condição: retorna a lista de segmentos
*/
Lista getSegmentos(Poligono p);

/*
Retorna a bounding box do polígono.
A bounding box é calculada e mantida atualizada internamente.

* p: ponteiro para o polígono

Pré-condição: p deve ser válido
Pós-condição: retorna a bounding box do polígono
*/
BoundingBox getBoundingBox(Poligono p);

/*
Retorna o número de vértices do polígono.

* p: ponteiro para o polígono

Pré-condição: p deve ser válido
Pós-condição: retorna o número de vértices
*/
int numeroVertices(Poligono p);

/*
Retorna o número de segmentos (arestas) do polígono.

* p: ponteiro para o polígono

Pré-condição: p deve ser válido
Pós-condição: retorna o número de segmentos
*/
int numeroSegmentos(Poligono p);

/*                    OPERAÇÕES GEOMÉTRICAS                    */

/*
Calcula o perímetro do polígono.

* p: ponteiro para o polígono

Pré-condição: p deve ser válido
Pós-condição: retorna o perímetro (soma dos comprimentos dos segmentos)
*/
double perimetroPoligono(Poligono p);

/*
Calcula a área do polígono usando a fórmula de Shoelace.
Funciona para polígonos simples (sem auto-interseção).

* p: ponteiro para o polígono

Pré-condição: p deve ser válido e ter pelo menos 3 vértices
Pós-condição: retorna a área do polígono
*/
double areaPoligono(Poligono p);

/*
Verifica se o polígono é convexo.
Um polígono é convexo se todos os ângulos internos são menores que 180°.

* p: ponteiro para o polígono

Pré-condição: p deve ser válido
Pós-condição: retorna true se o polígono é convexo
*/
bool poligonoConvexo(Poligono p);

/*                    CONSTRUÇÃO AUTOMÁTICA                    */

/*
Fecha o polígono conectando automaticamente vértices consecutivos
com segmentos. Cria segmentos entre cada par de vértices adjacentes
e entre o último e o primeiro vértice.

* p: ponteiro para o polígono

Pré-condição: p deve ser válido e ter pelo menos 2 vértices
Pós-condição: segmentos são criados conectando os vértices
*/
void fechaPoligono(Poligono p);

#endif