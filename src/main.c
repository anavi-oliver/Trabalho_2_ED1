#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "processaGeo.h"
#include "processaQry.h"
#include "svg.h"
#include "lista.h"
#include "formas.h"
#include "gerador.h"

#define PATH_LEN 512
#define FILE_NAME_LEN 256

/*________________________________ FUNÇÕES AUXILIARES ________________________________*/

static void trataPath(char *path, int tamMax, char* arg) {
    int argLen = strlen(arg);
    
    if (argLen >= tamMax) {
        fprintf(stderr, "ERRO: Caminho excede o tamanho máximo permitido.\n");
        exit(EXIT_FAILURE);
    }
    
    if (argLen > 0 && arg[argLen - 1] == '/') {
        arg[argLen - 1] = '\0';
        argLen--;
    }
    
    strncpy(path, arg, tamMax - 1);
    path[tamMax - 1] = '\0';
}

static char* montaCaminhoCompleto(const char *dir, const char *nomeArquivo) {
    if (!dir || !nomeArquivo || strlen(nomeArquivo) == 0) return NULL;
    
    size_t len = strlen(dir) + strlen(nomeArquivo) + 2;
    char *caminhoCompleto = (char*)malloc(len);
    
    if (caminhoCompleto == NULL) {
        fprintf(stderr, "ERRO: Falha ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }
    
    snprintf(caminhoCompleto, len, "%s/%s", dir, nomeArquivo);
    return caminhoCompleto;
}

static void getNomeBase(const char *nomeCompleto, char *destino, int tamMax) {
    // Primeiro, pega apenas o nome do arquivo (remove diretórios)
    const char *ultimaBarra = strrchr(nomeCompleto, '/');
    const char *nomeArquivo = ultimaBarra ? (ultimaBarra + 1) : nomeCompleto;
    
    // Copia para o destino
    strncpy(destino, nomeArquivo, tamMax);
    destino[tamMax - 1] = '\0';
    
    // Remove a extensão
    char *ponto = strrchr(destino, '.');
    if (ponto != NULL && ponto != destino) {
        *ponto = '\0';
    }
}

/*________________________________ MAIN ________________________________*/

int main(int argc, char *argv[]) {
    char dirEntrada[PATH_LEN] = ".";
    char arqGeo[FILE_NAME_LEN] = "";
    char arqQry[FILE_NAME_LEN] = "";
    char dirSaida[PATH_LEN] = "";
    
    bool f_encontrado = false;
    bool o_encontrado = false;
    
    // Parse argumentos
    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-e") == 0) {
            i++;
            if (i >= argc) {
                fprintf(stderr, "ERRO: -e requer um caminho.\n");
                return EXIT_FAILURE;
            }
            trataPath(dirEntrada, PATH_LEN, argv[i]);
        }
        else if (strcmp(argv[i], "-f") == 0) {
            i++;
            if (i >= argc) {
                fprintf(stderr, "ERRO: -f requer um arquivo .geo.\n");
                return EXIT_FAILURE;
            }
            strncpy(arqGeo, argv[i], FILE_NAME_LEN - 1);
            arqGeo[FILE_NAME_LEN - 1] = '\0';
            f_encontrado = true;
        }
        else if (strcmp(argv[i], "-q") == 0) {
            i++;
            if (i >= argc) {
                fprintf(stderr, "ERRO: -q requer um arquivo .qry.\n");
                return EXIT_FAILURE;
            }
            strncpy(arqQry, argv[i], FILE_NAME_LEN - 1);
            arqQry[FILE_NAME_LEN - 1] = '\0';
        }
        else if (strcmp(argv[i], "-o") == 0) {
            i++;
            if (i >= argc) {
                fprintf(stderr, "ERRO: -o requer um caminho.\n");
                return EXIT_FAILURE;
            }
            trataPath(dirSaida, PATH_LEN, argv[i]);
            o_encontrado = true;
        }
        i++;
    }
    
    if (!f_encontrado || !o_encontrado) {
        fprintf(stderr, "ERRO: -f e -o são obrigatórios.\n");
        fprintf(stderr, "Uso: %s -f arquivo.geo -o dir_saida [-e dir_entrada] [-q arquivo.qry]\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    // Processa arquivo .geo
    char *caminhoCompletoGeo = montaCaminhoCompleto(dirEntrada, arqGeo);
    Lista formas = processaArquivoGeo(caminhoCompletoGeo);
    free(caminhoCompletoGeo);
    
    if (formas == NULL) {
        fprintf(stderr, "ERRO: Falha ao processar .geo\n");
        return EXIT_FAILURE;
    }
    
    // Gera SVG inicial
    char nomeBaseGeo[FILE_NAME_LEN];
    getNomeBase(arqGeo, nomeBaseGeo, FILE_NAME_LEN);
    
    char nomeSvgInicial[FILE_NAME_LEN];
    snprintf(nomeSvgInicial, FILE_NAME_LEN, "%s.svg", nomeBaseGeo);
    char *caminhoSvgInicial = montaCaminhoCompleto(dirSaida, nomeSvgInicial);
    
    geraSVGCompleto(caminhoSvgInicial, formas, 800, 600);
    free(caminhoSvgInicial);
    
   // Processa arquivo .qry se fornecido
    if (arqQry[0] != '\0') {
        // Inicializa gerador de IDs
        int maiorId = calculaMaiorId(formas);
        Gerador gerador = criaGerador(maiorId + 1);
        
        // Monta nome base para arquivos de saída
        char nomeBaseQry[FILE_NAME_LEN];
        getNomeBase(arqQry, nomeBaseQry, FILE_NAME_LEN);
        
        char nomeBaseSaida[FILE_NAME_LEN * 2];
        snprintf(nomeBaseSaida, sizeof(nomeBaseSaida), "%s-%s", nomeBaseGeo, nomeBaseQry);
        
        printf("DEBUG MAIN: nomeBaseGeo='%s', nomeBaseQry='%s', nomeBaseSaida='%s'\n", 
               nomeBaseGeo, nomeBaseQry, nomeBaseSaida);
        
        // Processa .qry
        char *caminhoCompletoQry = montaCaminhoCompleto(dirEntrada, arqQry);
        processaArquivoQry(caminhoCompletoQry, formas, gerador, dirSaida, nomeBaseSaida);
        free(caminhoCompletoQry);
        
        destroiGerador(gerador);
    }
    
    // Libera memória
    destroiListaCompleta(formas, (void (*)(void*))destroiForma);
    
    printf("Processamento concluído.\n");
    return EXIT_SUCCESS;
}