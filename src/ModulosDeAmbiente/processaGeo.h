#ifndef PROCESSAGEO_H
#define PROCESSAGEO_H

#include "lista.h"
#include "texto.h"

/*
*        MÓDULO DE PROCESSAMENTO DO ARQUIVO .GEO
*
*        Este módulo processa o arquivo .geo que contém comandos para
*        criar formas geométricas (círculos, retângulos, linhas, textos).
*        
*        Comandos suportados:
*        - c: círculo
*        - r: retângulo
*        - l: linha
*        - t: texto
*        - ts: estilo de texto
*/

/*
Processa o arquivo .geo e retorna lista de formas criadas.

Lê o arquivo linha por linha, interpretando os comandos e criando
as formas geométricas correspondentes.

* caminhoArquivo: caminho completo do arquivo .geo

Pré-condição: caminhoArquivo deve apontar para arquivo válido
Pós-condição: retorna lista com todas as formas criadas,
              ou NULL em caso de erro
*/
Lista processaArquivoGeo(const char *caminhoArquivo);

/*
Processa uma linha de comando do arquivo .geo.

* linha: string com o comando a processar
* formas: lista onde a forma criada será adicionada
* estiloAtual: estilo de texto corrente

Pré-condição: linha e formas devem ser válidos
Pós-condição: forma criada e adicionada à lista se comando válido
*/
void processaLinha(char *linha, Lista formas, Estilo estiloAtual);

/*
Busca uma forma na lista pelo ID.

* formas: lista de formas
* id: identificador da forma procurada

Pré-condição: formas deve ser válida
Pós-condição: retorna a forma com o ID, ou NULL se não encontrada
*/
void* buscaFormaPorId(Lista formas, int id);

/*
Remove uma forma da lista pelo ID.

* formas: lista de formas
* id: identificador da forma a remover

Pré-condição: formas deve ser válida
Pós-condição: forma removida se existia, retorna true se removeu
*/
bool removeFormaPorId(Lista formas, int id);

/*
Calcula o maior ID presente na lista de formas.
Útil para inicializar o gerador de IDs.

* formas: lista de formas

Pré-condição: formas deve ser válida
Pós-condição: retorna o maior ID, ou 0 se lista vazia
*/
int calculaMaiorId(Lista formas);

#endif