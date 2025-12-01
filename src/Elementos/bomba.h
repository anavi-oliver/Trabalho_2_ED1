#ifndef BOMBA_H
#define BOMBA_H

#include "lista.h"
#include "poligono.h"
#include "gerador.h"
#include "formas.h"
#include <stdio.h>

/*
*        MÓDULO DE BOMBAS
*
*        Implementa os 3 tipos de bombas alienígenas:
*        - Destruição: remove formas na região de visibilidade
*        - Pintura: muda cores das formas na região
*        - Clonagem: duplica formas e as translada
*        
*        A região de visibilidade é um polígono calculado considerando
*        os anteparos que bloqueiam a propagação da explosão.
*/

/*                    BOMBA DE DESTRUIÇÃO                    */

/*
Processa bomba de destruição na coordenada (x, y).

Calcula região de visibilidade e destrói formas que estão
(parcialmente) dentro dessa região.

* x, y: coordenadas da explosão
* formas: lista de formas
* arquivoTxt: arquivo para reportar destruições
* arquivoSvg: arquivo SVG para desenhar região
* sufixo: sufixo do arquivo ("-" para não criar arquivo separado)

Pré-condição: formas deve ser válida
Pós-condição: formas destruídas removidas da lista, relatórios gerados
*/
void processaBombaDestruicao(double x, double y, Lista formas, FILE *arquivoTxt, const char *sufixo, const char *caminhoSaida, const char *nomeBase);

/*                    BOMBA DE PINTURA                    */

/*
Processa bomba de pintura na coordenada (x, y).

Calcula região de visibilidade e pinta formas que estão
(parcialmente) dentro dessa região com a cor especificada.

* x, y: coordenadas da explosão
* cor: nova cor para as formas
* formas: lista de formas
* arquivoTxt: arquivo para reportar pinturas
* sufixo: sufixo do arquivo
* caminhoSaida, nomeBase: para gerar arquivo SVG se necessário

Pré-condição: formas e cor devem ser válidos
Pós-condição: formas pintadas com nova cor, relatórios gerados
*/
void processaBombaPintura(double x, double y, const char *cor, Lista formas, FILE *arquivoTxt, const char *sufixo, const char *caminhoSaida, const char *nomeBase);

/*                    BOMBA DE CLONAGEM                    */

/*
Processa bomba de clonagem na coordenada (x, y).

Calcula região de visibilidade e clona formas que estão
(parcialmente) dentro dessa região. Os clones são transladados
por (dx, dy) e recebem IDs únicos.

* x, y: coordenadas da explosão
* dx, dy: deslocamento dos clones
* formas: lista de formas
* gerador: gerador de IDs para os clones
* arquivoTxt: arquivo para reportar clonagens
* sufixo: sufixo do arquivo
* caminhoSaida, nomeBase: para gerar arquivo SVG se necessário

Pré-condição: formas e gerador devem ser válidos
Pós-condição: clones criados e adicionados à lista, relatórios gerados
*/
void processaBombaClonagem(double x, double y, double dx, double dy, Lista formas, Gerador gerador, FILE *arquivoTxt, const char *sufixo, const char *caminhoSaida,  const char *nomeBase);

/*                    FUNÇÕES AUXILIARES                    */

/*
Calcula a região de visibilidade a partir de um ponto.

Usa algoritmo de varredura angular considerando anteparos
que bloqueiam a visão.

* x, y: coordenadas do ponto de origem
* formas: lista de formas (incluindo anteparos)

Pré-condição: formas deve ser válida
Pós-condição: retorna polígono representando região visível
*/
Poligono calculaRegiaoVisibilidade(double x, double y, Lista formas);

/*
Verifica se uma forma está (parcialmente) dentro do polígono.

Usa os critérios da Figura 3 do projeto:
- Círculo: centro dentro OU aresta próxima (dist <= r) OU vértice dentro
- Retângulo: vértice do retângulo dentro OU vértice do polígono dentro 
              OU aresta intersecta aresta
- Linha: extremidade dentro OU intersecta aresta do polígono
- Texto: convertido para linha e testado como linha

* forma: forma a testar
* regiao: polígono da região de visibilidade

Pré-condição: forma e regiao devem ser válidos
Pós-condição: retorna true se forma está (parcialmente) dentro
*/
bool formaNoPoligono(Forma forma, Poligono regiao);


#endif