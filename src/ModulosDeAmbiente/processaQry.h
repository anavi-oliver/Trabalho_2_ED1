// ============= processaQry.h =============
#ifndef PROCESSAQRY_H
#define PROCESSAQRY_H

#include "lista.h"
#include "gerador.h"
#include <stdio.h>

/*
Processa o arquivo .qry executando comandos de bomba e anteparo.

* caminhoArquivo: caminho completo do arquivo .qry
* formas: lista de formas (modificada pelos comandos)
* gerador: gerador de IDs
* caminhoSaida: diretório de saída para arquivos gerados
* nomeBase: nome base para arquivos de saída

Pré-condição: todos os parâmetros devem ser válidos
Pós-condição: comandos executados, arquivos .txt e .svg gerados
*/
void processaArquivoQry(const char *caminhoArquivo, Lista formas, 
                        Gerador gerador, const char *caminhoSaida, 
                        const char *nomeBase);

#endif
