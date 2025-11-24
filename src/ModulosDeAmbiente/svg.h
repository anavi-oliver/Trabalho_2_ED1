#ifndef SVG_H
#define SVG_H

#include <stdio.h>
#include "lista.h"
#include "poligono.h"

/*
*        MÓDULO DE GERAÇÃO SVG
*
*        Este módulo é responsável por gerar arquivos no formato SVG
*        (Scalable Vector Graphics) contendo as formas geométricas.
*        
*        Produz arquivos .svg após processamento de .geo e .qry
*/

/*                    GERAÇÃO DE ARQUIVOS SVG                    */

/*
Abre um arquivo SVG e escreve o cabeçalho.

* nomeArquivo: caminho completo do arquivo a ser criado
* largura: largura da área de desenho
* altura: altura da área de desenho

Pré-condição: nomeArquivo deve ser válido
Pós-condição: retorna FILE* aberto para escrita, ou NULL em caso de erro
*/
FILE* abreSVG(const char *nomeArquivo, double largura, double altura);

/*
Fecha um arquivo SVG escrevendo a tag de fechamento.

* arquivo: ponteiro para o arquivo SVG

Pré-condição: arquivo deve ser válido e aberto para escrita
Pós-condição: tag </svg> escrita e arquivo fechado
*/
void fechaSVG(FILE *arquivo);

/*
Escreve uma lista de formas em um arquivo SVG.
Usa a função desenhaForma() de cada forma.

* arquivo: ponteiro para o arquivo SVG
* formas: lista de formas geométricas

Pré-condição: arquivo e formas devem ser válidos
Pós-condição: todas as formas são desenhadas no SVG
*/
void escreveFormasSVG(FILE *arquivo, Lista formas);

/*
Desenha um polígono (região de visibilidade) no SVG.

* arquivo: ponteiro para o arquivo SVG
* poli: polígono a ser desenhado
* cor: cor de preenchimento
* opacidade: opacidade (0.0 a 1.0)

Pré-condição: arquivo e poli devem ser válidos
Pós-condição: polígono desenhado no SVG
*/
void desenhaPoligonoSVG(FILE *arquivo, Poligono poli, const char *cor, double opacidade);

/*
Desenha um círculo de explosão (indicador visual da bomba).

* arquivo: ponteiro para o arquivo SVG
* x, y: coordenadas do centro
* raio: raio do círculo
* cor: cor do círculo

Pré-condição: arquivo deve ser válido
Pós-condição: círculo desenhado no SVG
*/
void desenhaExplosaoSVG(FILE *arquivo, double x, double y, double raio, const char *cor);

/*
Desenha um texto no SVG (útil para anotações).

* arquivo: ponteiro para o arquivo SVG
* x, y: coordenadas do texto
* texto: string a ser desenhada
* cor: cor do texto
* tamanho: tamanho da fonte

Pré-condição: arquivo e texto devem ser válidos
Pós-condição: texto desenhado no SVG
*/
void desenhaTextoSVG(FILE *arquivo, double x, double y, const char *texto, 
                     const char *cor, int tamanho);

/*
Gera arquivo SVG completo a partir de lista de formas.

* nomeArquivo: caminho do arquivo a ser criado
* formas: lista de formas a desenhar
* largura, altura: dimensões do SVG

Pré-condição: nomeArquivo e formas devem ser válidos
Pós-condição: arquivo SVG completo criado
*/
void geraSVGCompleto(const char *nomeArquivo, Lista formas, 
                     double largura, double altura);

#endif