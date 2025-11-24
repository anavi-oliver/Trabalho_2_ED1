#ifndef ANTEPARO_H
#define ANTEPARO_H

#include "lista.h"
#include "formas.h"
#include "gerador.h"

/*
*        MÓDULO DE ANTEPAROS
*
*        Anteparos são formas geométricas pintadas com tinta bloqueante
*        que impedem a propagação da explosão das bombas.
*        
*        Transformações:
*        - Círculo → segmento horizontal ou vertical no diâmetro
*        - Retângulo → 4 segmentos (seus lados)
*        - Texto → segmento (conforme âncora)
*        - Linha → permanece linha (apenas marcada como anteparo)
*/

/*
Transforma uma forma em anteparo(s).

A forma original é removida e substituída por segmento(s) de linha.
Os novos segmentos são marcados como anteparos.

* forma: forma a ser transformada
* orientacao: 'h' horizontal ou 'v' vertical (para círculos)
* gerador: gerador de IDs para os novos segmentos
* formas: lista onde os anteparos serão adicionados

Pré-condição: forma, gerador e formas devem ser válidos
Pós-condição: forma transformada em segmentos anteparos
*/
void transformaEmAnteparo(Forma forma, char orientacao, Gerador gerador, Lista formas);

/*
Transforma círculo em segmento anteparo.

* c: círculo a transformar
* orientacao: 'h' para horizontal, 'v' para vertical
* idSegmento: ID do novo segmento
* formas: lista onde adicionar o segmento

Pré-condição: c, formas válidos
Pós-condição: segmento criado e adicionado
*/
void circuloParaAnteparo(void *c, char orientacao, int idSegmento, Lista formas);

/*
Transforma retângulo em 4 segmentos anteparos (seus lados).

* r: retângulo a transformar
* gerador: gerador para IDs dos 4 segmentos
* formas: lista onde adicionar segmentos

Pré-condição: r, gerador, formas válidos
Pós-condição: 4 segmentos criados (topo, direita, baixo, esquerda)
*/
void retanguloParaAnteparo(void *r, Gerador gerador, Lista formas);

/*
Transforma texto em segmento anteparo.

O texto é convertido em linha horizontal conforme sua âncora:
- 'i': início em (x,y), fim em (x + 10*|texto|, y)
- 'f': início em (x - 10*|texto|, y), fim em (x, y)
- 'm': início em (x - 5*|texto|, y), fim em (x + 5*|texto|, y)

* t: texto a transformar
* idSegmento: ID do novo segmento
* formas: lista onde adicionar

Pré-condição: t, formas válidos
Pós-condição: segmento criado
*/
void textoParaAnteparo(void *t, int idSegmento, Lista formas);

/*
Processa comando 'a' do arquivo .qry.

Transforma todas as formas no intervalo [idInicio, idFim] em anteparos.

* formas: lista de formas
* idInicio: ID inicial do intervalo
* idFim: ID final do intervalo (inclusivo)
* orientacao: 'h' ou 'v' para círculos
* gerador: gerador de IDs
* arquivoTxt: arquivo para reportar transformações

Pré-condição: formas, gerador e arquivoTxt válidos
Pós-condição: formas transformadas, relatório escrito
*/
void processaComandoAnteparo(Lista formas, int idInicio, int idFim, char orientacao, Gerador gerador, FILE *arquivoTxt);

#endif