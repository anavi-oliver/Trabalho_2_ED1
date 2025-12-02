#include "processaQry.h"
#include "anteparo.h"
#include "bomba.h"
#include "svg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA 1000

static char* montaCaminho(const char *dir, const char *arquivo) {
    size_t len = strlen(dir) + strlen(arquivo) + 2;
    char *caminho = (char*)malloc(len);
    if (caminho) {
        snprintf(caminho, len, "%s/%s", dir, arquivo);
    }
    return caminho;
}

void processaArquivoQry(const char *caminhoArquivo, Lista formas, 
                        Gerador gerador, const char *caminhoSaida, 
                        const char *nomeBase) {
    if (!caminhoArquivo || !formas || !gerador || !caminhoSaida || !nomeBase) {
        return;
    }
    
    FILE *arquivoQry = fopen(caminhoArquivo, "r");
    if (!arquivoQry) {
        fprintf(stderr, "ERRO: Não foi possível abrir o arquivo .qry: %s\n", caminhoArquivo);
        return;
    }
    
    // Cria arquivo .txt de saída
    char nomeArquivoTxt[512];
    snprintf(nomeArquivoTxt, sizeof(nomeArquivoTxt), "%s.txt", nomeBase);
    char *caminhoTxt = montaCaminho(caminhoSaida, nomeArquivoTxt);
    FILE *arquivoTxt = fopen(caminhoTxt, "w");
    free(caminhoTxt);
    
    if (!arquivoTxt) {
        fprintf(stderr, "ERRO: Não foi possível criar arquivo .txt de saída\n");
        fclose(arquivoQry);
        return;
    }
    
    char linha[MAX_LINHA];
    int contadorComandos = 0;
    
    while (fgets(linha, MAX_LINHA, arquivoQry)) {
        // Remove newline
        linha[strcspn(linha, "\n")] = '\0';
        
        // Ignora linhas vazias e comentários
        if (strlen(linha) == 0 || linha[0] == '#') {
            continue;
        }
        
        char comando;
        if (sscanf(linha, " %c", &comando) != 1) {
            continue;
        }
        
        // Comando 'a': Anteparo
        if (comando == 'a') {
            int idInicio, idFim;
            char orientacao;
            
            if (sscanf(linha, "a %d %d %c", &idInicio, &idFim, &orientacao) == 3) {
                processaComandoAnteparo(formas, idInicio, idFim, orientacao, gerador, arquivoTxt);
                
                // Gera SVG após anteparo
                contadorComandos++;
                char nomeSvg[512];
                snprintf(nomeSvg, sizeof(nomeSvg), "%s-a%d%d%c.svg", nomeBase, idInicio, idFim, orientacao);
                char *caminhoSvg = montaCaminho(caminhoSaida, nomeSvg);
                geraSVGCompleto(caminhoSvg, formas, 800, 600);
                free(caminhoSvg);
            }
        }
        // Comando 'd': Bomba de destruição
        else if (comando == 'd') {
            double x, y;
            char sufixo[256];
            
            if (sscanf(linha, "d %lf %lf %s", &x, &y, sufixo) == 3) {
                processaBombaDestruicao(x, y, formas, arquivoTxt, sufixo, caminhoSaida, nomeBase);
                
                // Gera SVG após destruição
                if (strcmp(sufixo, "-") != 0) {
                    char nomeSvg[512];
                    snprintf(nomeSvg, sizeof(nomeSvg), "%s-d%.0f%.0f%s.svg", nomeBase, x, y, sufixo);
                    char *caminhoSvg = montaCaminho(caminhoSaida, nomeSvg);
                    geraSVGCompleto(caminhoSvg, formas, 800, 600);
                    free(caminhoSvg);
                }
            }
        }
        // Comando 'p': Bomba de pintura
        else if (comando == 'p') {
            double x, y;
            char cor[128], sufixo[256];
            
            if (sscanf(linha, "p %lf %lf %s %s", &x, &y, cor, sufixo) == 4) {
                processaBombaPintura(x, y, cor, formas, arquivoTxt, sufixo, caminhoSaida, nomeBase);
                
                // Gera SVG após pintura
                if (strcmp(sufixo, "-") != 0) {
                    char nomeSvg[512];
                    snprintf(nomeSvg, sizeof(nomeSvg), "%s-p%.0f%.0f%s.svg", nomeBase, x, y, sufixo);
                    char *caminhoSvg = montaCaminho(caminhoSaida, nomeSvg);
                    geraSVGCompleto(caminhoSvg, formas, 800, 600);
                    free(caminhoSvg);
                }
            }
        }
        // Comando 'cln': Bomba de clonagem
        else if (strncmp(linha, "cln", 3) == 0) {
            double x, y, dx, dy;
            char sufixo[256];
            
            if (sscanf(linha, "cln %lf %lf %lf %lf %s", &x, &y, &dx, &dy, sufixo) == 5) {
                processaBombaClonagem(x, y, dx, dy, formas, gerador, arquivoTxt, sufixo, caminhoSaida, nomeBase);
                
                // Gera SVG após clonagem
                if (strcmp(sufixo, "-") != 0) {
                    char nomeSvg[512];
                    snprintf(nomeSvg, sizeof(nomeSvg), "%s-cln%.0f%.0f.svg", nomeBase, x, y);
                    char *caminhoSvg = montaCaminho(caminhoSaida, nomeSvg);
                    geraSVGCompleto(caminhoSvg, formas, 800, 600);
                    free(caminhoSvg);
                }
            }
        }
    }
    
    fclose(arquivoQry);
    fclose(arquivoTxt);
    
    printf("Consultas processadas. Resultados em: %s/%s.txt\n", caminhoSaida, nomeBase);
}