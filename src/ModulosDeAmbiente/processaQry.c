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
        fprintf(stderr, "ERRO: Parâmetros nulos em processaArquivoQry\n");
        return;
    }
    
    printf("DEBUG: Abrindo arquivo .qry: %s\n", caminhoArquivo);
    
    FILE *arquivoQry = fopen(caminhoArquivo, "r");
    if (!arquivoQry) {
        fprintf(stderr, "ERRO: Não foi possível abrir o arquivo .qry: %s\n", caminhoArquivo);
        return;
    }
    
    // Cria arquivo .txt de saída
    char nomeArquivoTxt[512];
    snprintf(nomeArquivoTxt, sizeof(nomeArquivoTxt), "%s.txt", nomeBase);
    char *caminhoTxt = montaCaminho(caminhoSaida, nomeArquivoTxt);
    
    printf("DEBUG: Criando arquivo .txt: %s\n", caminhoTxt);
    
    FILE *arquivoTxt = fopen(caminhoTxt, "w");
    
    if (!arquivoTxt) {
        fprintf(stderr, "ERRO: Não foi possível criar arquivo .txt de saída: %s\n", caminhoTxt);
        fprintf(stderr, "      Verifique se o diretório existe e você tem permissão de escrita.\n");
        free(caminhoTxt);
        fclose(arquivoQry);
        return;
    }
    
    free(caminhoTxt);
    
    char linha[MAX_LINHA];
    int contadorComandos = 0;
    int numeroLinha = 0;
    
    while (fgets(linha, MAX_LINHA, arquivoQry)) {
        numeroLinha++;
        
        // Remove newline
        linha[strcspn(linha, "\n")] = '\0';
        
        printf("DEBUG: Linha %d lida: '%s'\n", numeroLinha, linha);
        
        // Ignora linhas vazias e comentários
        if (strlen(linha) == 0 || linha[0] == '#') {
            printf("DEBUG: Linha ignorada (vazia ou comentário)\n");
            continue;
        }
        
        char comando;
        if (sscanf(linha, " %c", &comando) != 1) {
            printf("DEBUG: Não foi possível ler comando\n");
            continue;
        }
        
        printf("DEBUG: Comando identificado: '%c'\n", comando);
        
        // Comando 'a': Anteparo
        if (comando == 'a') {
            int idInicio, idFim;
            char orientacao;
            
            if (sscanf(linha, "a %d %d %c", &idInicio, &idFim, &orientacao) == 3) {
                printf("DEBUG: Processando anteparo: %d %d %c\n", idInicio, idFim, orientacao);
                
                processaComandoAnteparo(formas, idInicio, idFim, orientacao, gerador, arquivoTxt);
                
                // Gera SVG após anteparo
                contadorComandos++;
                char nomeSvg[512];
                snprintf(nomeSvg, sizeof(nomeSvg), "%s-a%d%d%c.svg", nomeBase, idInicio, idFim, orientacao);
                char *caminhoSvg = montaCaminho(caminhoSaida, nomeSvg);
                
                printf("DEBUG: Gerando SVG: %s\n", caminhoSvg);
                geraSVGCompleto(caminhoSvg, formas, 800, 600);
                printf("DEBUG: SVG gerado com sucesso!\n");
                
                free(caminhoSvg);
            } else {
                printf("DEBUG: ERRO ao fazer parse do comando anteparo\n");
            }
        }
        // Comando 'd': Bomba de destruição
        else if (comando == 'd') {
            double x, y;
            char sufixo[256];
            
            if (sscanf(linha, "d %lf %lf %s", &x, &y, sufixo) == 3) {
                printf("DEBUG: Processando bomba destruição: %.2f %.2f %s\n", x, y, sufixo);
                
                // PRIMEIRO: Gera SVG com visualização da bomba (ANTES de destruir)
                if (strcmp(sufixo, "-") != 0) {
                    contadorComandos++;
                    char nomeSvg[512];
                    snprintf(nomeSvg, sizeof(nomeSvg), "%s-d%.0f%.0f%s.svg", nomeBase, x, y, sufixo);
                    char *caminhoSvg = montaCaminho(caminhoSaida, nomeSvg);
                    
                    printf("DEBUG: Gerando SVG com visualização da bomba: %s\n", caminhoSvg);
                    
                    // Cria arquivo SVG manualmente para adicionar polígono
                    FILE *svg = abreSVG(caminhoSvg, 800, 600);
                    if (svg != NULL) {
                        // Desenha formas existentes
                        escreveFormasSVG(svg, formas);
                        
                        // Desenha polígono de visibilidade
                        Poligono regiao = calculaRegiaoVisibilidade(x, y, formas);
                        desenhaPoligonoSVG(svg, regiao, "red", 0.2);
                        destroiPoligono(regiao);
                        
                        // Desenha explosão
                        desenhaExplosaoSVG(svg, x, y, 10, "red");
                        
                        fechaSVG(svg);
                    }
                    
                    printf("DEBUG: SVG com visualização gerado!\n");
                    free(caminhoSvg);
                }
                
                // DEPOIS: Processa a destruição
                processaBombaDestruicao(x, y, formas, arquivoTxt, sufixo, caminhoSaida, nomeBase);
                
            } else {
                printf("DEBUG: ERRO ao fazer parse do comando bomba destruição\n");
            }
        }
        // Comando 'p': Bomba de pintura
        else if (comando == 'p') {
            double x, y;
            char cor[128], sufixo[256];
            
            if (sscanf(linha, "p %lf %lf %s %s", &x, &y, cor, sufixo) == 4) {
                printf("DEBUG: Processando bomba pintura: %.2f %.2f %s %s\n", x, y, cor, sufixo);
                
                // Gera SVG com visualização (ANTES de pintar)
                if (strcmp(sufixo, "-") != 0) {
                    char nomeSvg[512];
                    snprintf(nomeSvg, sizeof(nomeSvg), "%s-p%.0f%.0f%s.svg", nomeBase, x, y, sufixo);
                    char *caminhoSvg = montaCaminho(caminhoSaida, nomeSvg);
                    
                    printf("DEBUG: Gerando SVG com visualização da pintura: %s\n", caminhoSvg);
                    
                    FILE *svg = abreSVG(caminhoSvg, 800, 600);
                    if (svg != NULL) {
                        escreveFormasSVG(svg, formas);
                        
                        Poligono regiao = calculaRegiaoVisibilidade(x, y, formas);
                        desenhaPoligonoSVG(svg, regiao, cor, 0.2);
                        destroiPoligono(regiao);
                        
                        desenhaExplosaoSVG(svg, x, y, 10, cor);
                        
                        fechaSVG(svg);
                    }
                    
                    free(caminhoSvg);
                }
                
                processaBombaPintura(x, y, cor, formas, arquivoTxt, sufixo, caminhoSaida, nomeBase);
                
            } else {
                printf("DEBUG: ERRO ao fazer parse do comando bomba pintura\n");
            }
        }
        // Comando 'cln': Bomba de clonagem
        else if (strncmp(linha, "cln", 3) == 0) {
            double x, y, dx, dy;
            char sufixo[256];
            
            if (sscanf(linha, "cln %lf %lf %lf %lf %s", &x, &y, &dx, &dy, sufixo) == 5) {
                printf("DEBUG: Processando bomba clonagem: %.2f %.2f %.2f %.2f %s\n", x, y, dx, dy, sufixo);
                
                // Gera SVG com visualização (ANTES de clonar)
                if (strcmp(sufixo, "-") != 0) {
                    char nomeSvg[512];
                    snprintf(nomeSvg, sizeof(nomeSvg), "%s-cln%.0f%.0f.svg", nomeBase, x, y);
                    char *caminhoSvg = montaCaminho(caminhoSaida, nomeSvg);
                    
                    printf("DEBUG: Gerando SVG com visualização da clonagem: %s\n", caminhoSvg);
                    
                    FILE *svg = abreSVG(caminhoSvg, 800, 600);
                    if (svg != NULL) {
                        escreveFormasSVG(svg, formas);
                        
                        Poligono regiao = calculaRegiaoVisibilidade(x, y, formas);
                        desenhaPoligonoSVG(svg, regiao, "blue", 0.2);
                        destroiPoligono(regiao);
                        
                        desenhaExplosaoSVG(svg, x, y, 10, "blue");
                        
                        fechaSVG(svg);
                    }
                    
                    free(caminhoSvg);
                }
                
                processaBombaClonagem(x, y, dx, dy, formas, gerador, arquivoTxt, sufixo, caminhoSaida, nomeBase);
                
            } else {
                printf("DEBUG: ERRO ao fazer parse do comando bomba clonagem\n");
            }
        } else {
            printf("DEBUG: Comando não reconhecido: '%c'\n", comando);
        }
    }
    
    fclose(arquivoQry);
    fclose(arquivoTxt);
    
    printf("DEBUG: Total de comandos processados: %d\n", contadorComandos);
    printf("Consultas processadas. Resultados em: %s/%s.txt\n", caminhoSaida, nomeBase);
}