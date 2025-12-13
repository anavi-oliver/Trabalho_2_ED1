#ifndef PROCESSAQRY_H
#define PROCESSAQRY_H

#include "lista.h"
#include "gerador.h"

/*
 * Processa o arquivo de consultas (.qry).
 * Lê comandos de bombas (d, p), calcula visibilidade e gera SVGs.
 * * entrada: Caminho do arquivo .qry
 * formas: Lista contendo todas as formas do cenário
 * gerador: Gerador de IDs (caso precise criar novas formas)
 * dirSaida: Diretório para salvar os SVGs
 * nomeBase: Nome base do arquivo geo (para compor nome da saída)
 * tipoSort: 'q' ou 'm' (QuickSort ou MergeSort)
 * threshold: Limite para o Insertion Sort
 */
void processaArquivoQry(const char* entrada, Lista formas, Gerador gerador, const char* dirSaida, const char* nomeBase, char tipoSort, int threshold);

#endif