#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Módulos do projeto
#include "segmento.h"
#include "ponto.h"
#include "visibilidade.h"
#include "svg.h"

#define PATH_LEN 512
#define FILE_NAME_LEN 256

/*________________________________ FUNÇÕES AUXILIARES DE CAMINHO ________________________________*/

static void trataPath(char *path, int tamMax, char* arg) {
    int argLen = strlen(arg);
    
    if (argLen >= tamMax) {
        fprintf(stderr, "ERRO: Caminho excede o tamanho máximo permitido.\n");
        exit(EXIT_FAILURE);
    }
    
    // Remove barra final se houver
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
        fprintf(stderr, "ERRO: Falha ao alocar memória para o caminho completo.\n");
        exit(EXIT_FAILURE);
    }
    
    sprintf(caminhoCompleto, "%s/%s", dir, nomeArquivo);
    return caminhoCompleto;
}

static void getNomeBase(const char *nomeCompleto, char *destino, int tamMax) {
    strncpy(destino, nomeCompleto, tamMax);
    destino[tamMax - 1] = '\0';
    char *ponto = strrchr(destino, '.');
    if (ponto != NULL && ponto != destino) {
        *ponto = '\0';
    }
}


/*________________________________ PROCESSAMENTO DE ARQUIVOS ________________________________*/

// Processa arquivo .geo (segmentos)
static Segmento* processaGeo(const char *caminhoArquivo, int *numSegmentos) {
    FILE *arquivo = fopen(caminhoArquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "ERRO: Não foi possível abrir o arquivo .geo: %s\n", caminhoArquivo);
        return NULL;
    }
    
    // Primeira passagem: conta segmentos
    int count = 0;
    char linha[512];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (linha[0] != '\n' && linha[0] != '#') {
            count++;
        }
    }
    
    // Aloca array de segmentos
    Segmento* segmentos = (Segmento*)malloc(count * sizeof(Segmento));
    if (segmentos == NULL) {
        fclose(arquivo);
        return NULL;
    }
    
    // Segunda passagem: lê segmentos
    rewind(arquivo);
    int idx = 0;
    
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (linha[0] == '\n' || linha[0] == '#') {
            continue;
        }
        
        int id;
        double x1, y1, x2, y2;
        char cor[64];
        
        if (sscanf(linha, "%d %lf %lf %lf %lf %s", &id, &x1, &y1, &x2, &y2, cor) == 6) {
            segmentos[idx] = criaSegmento(id, x1, y1, x2, y2, cor);
            idx++;
        }
    }
    
    fclose(arquivo);
    *numSegmentos = idx;
    return segmentos;
}

// Processa arquivo .qry (consultas de visibilidade)
static void processaQry(const char *caminhoQry, const char *caminhoTxt,
                       const char *caminhoSvg, Segmento* segmentos, int numSegmentos) {
    FILE *arquivoQry = fopen(caminhoQry, "r");
    if (arquivoQry == NULL) {
        fprintf(stderr, "ERRO: Não foi possível abrir o arquivo .qry: %s\n", caminhoQry);
        return;
    }
    
    FILE *arquivoTxt = fopen(caminhoTxt, "w");
    if (arquivoTxt == NULL) {
        fprintf(stderr, "ERRO: Não foi possível criar o arquivo .txt: %s\n", caminhoTxt);
        fclose(arquivoQry);
        return;
    }
    
    fprintf(arquivoTxt, "========== PROCESSAMENTO DE CONSULTAS ==========\n\n");
    
    char linha[512];
    int numConsultas = 0;
    
    // Processa cada consulta
    while (fgets(linha, sizeof(linha), arquivoQry) != NULL) {
        if (linha[0] == '\n' || linha[0] == '#') {
            continue;
        }
        
        numConsultas++;
        fprintf(arquivoTxt, "[Consulta %d] %s", numConsultas, linha);
        
        // Extrai coordenadas do ponto de observação
        double px, py;
        if (sscanf(linha, "%lf %lf", &px, &py) == 2) {
            // Calcula visibilidade
            bool* segmentosVisiveis = (bool*)malloc(numSegmentos * sizeof(bool));
            if (segmentosVisiveis != NULL) {
                calculaVisibilidade(px, py, segmentos, numSegmentos, segmentosVisiveis);
                
                // Imprime relatório
                imprimeRelatorioVisibilidade(arquivoTxt, segmentos, segmentosVisiveis, 
                                            numSegmentos, px, py);
                
                // Gera SVG para esta consulta
                char caminhoSvgConsulta[1024];
                sprintf(caminhoSvgConsulta, "%s-consulta%d.svg", caminhoSvg, numConsultas);
                
                FILE* svgFile = inicializaSVG(caminhoSvgConsulta, 800, 600);
                if (svgFile != NULL) {
                    // Desenha segmentos (visíveis em verde, ocultos em vermelho)
                    for (int i = 0; i < numSegmentos; i++) {
                        const char* cor = segmentosVisiveis[i] ? "green" : "red";
                        desenhaSegmentoSVG(svgFile, segmentos[i], cor, 2.0);
                    }
                    
                    // Desenha ponto de observação
                    desenhaPontoSVG(svgFile, px, py, "blue", 5.0);
                    
                    finalizaSVG(svgFile);
                }
                
                free(segmentosVisiveis);
            }
        } else {
            fprintf(arquivoTxt, "ERRO: Formato de consulta inválido.\n\n");
        }
    }
    
    fprintf(arquivoTxt, "\n========== RESUMO ==========\n");
    fprintf(arquivoTxt, "Total de consultas processadas: %d\n", numConsultas);
    fprintf(arquivoTxt, "Total de segmentos analisados: %d\n", numSegmentos);
    fprintf(arquivoTxt, "============================\n");
    
    fclose(arquivoQry);
    fclose(arquivoTxt);
}


/*________________________________ FUNÇÃO MAIN ________________________________*/

int main(int argc, char *argv[]) {
    // Variáveis de caminhos
    char dirEntrada[PATH_LEN] = ".";
    char arqGeo[FILE_NAME_LEN] = "";
    char arqQry[FILE_NAME_LEN] = "";
    char dirSaida[PATH_LEN] = "";
    
    // Flags de parâmetros obrigatórios
    bool f_encontrado = false;
    bool o_encontrado = false;
    
    // Tratamento dos parâmetros CLI
    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-e") == 0) {
            i++;
            if (i >= argc) {
                fprintf(stderr, "ERRO: O parâmetro -e requer um caminho.\n");
                return EXIT_FAILURE;
            }
            trataPath(dirEntrada, PATH_LEN, argv[i]);
        }
        else if (strcmp(argv[i], "-f") == 0) {
            i++;
            if (i >= argc) {
                fprintf(stderr, "ERRO: O parâmetro -f requer um nome de arquivo .geo.\n");
                return EXIT_FAILURE;
            }
            strncpy(arqGeo, argv[i], FILE_NAME_LEN - 1);
            arqGeo[FILE_NAME_LEN - 1] = '\0';
            f_encontrado = true;
        }
        else if (strcmp(argv[i], "-q") == 0) {
            i++;
            if (i >= argc) {
                fprintf(stderr, "ERRO: O parâmetro -q requer um nome de arquivo .qry.\n");
                return EXIT_FAILURE;
            }
            strncpy(arqQry, argv[i], FILE_NAME_LEN - 1);
            arqQry[FILE_NAME_LEN - 1] = '\0';
        }
        else if (strcmp(argv[i], "-o") == 0) {
            i++;
            if (i >= argc) {
                fprintf(stderr, "ERRO: O parâmetro -o requer um caminho de saída.\n");
                return EXIT_FAILURE;
            }
            trataPath(dirSaida, PATH_LEN, argv[i]);
            o_encontrado = true;
        }
        else {
            fprintf(stderr, "AVISO: Parâmetro desconhecido ignorado: %s\n", argv[i]);
        }
        i++;
    }
    
    // Validação de parâmetros obrigatórios
    if (!f_encontrado || !o_encontrado) {
        fprintf(stderr, "ERRO: Os parâmetros -f e -o são obrigatórios.\n");
        fprintf(stderr, "Uso: %s -f arquivo.geo -o dir_saida [-e dir_entrada] [-q arquivo.qry]\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    // Processa arquivo .geo
    char *caminhoCompletoGeo = montaCaminhoCompleto(dirEntrada, arqGeo);
    int numSegmentos = 0;
    Segmento* segmentos = processaGeo(caminhoCompletoGeo, &numSegmentos);
    free(caminhoCompletoGeo);
    
    if (segmentos == NULL) {
        fprintf(stderr, "ERRO: Falha ao processar o arquivo .geo\n");
        return EXIT_FAILURE;
    }
    
    printf("Arquivo .geo processado: %d segmentos carregados.\n", numSegmentos);
    
    // Gera SVG inicial (todos os segmentos)
    char nomeBaseGeo[FILE_NAME_LEN];
    getNomeBase(arqGeo, nomeBaseGeo, FILE_NAME_LEN);
    
    char nomeSvgInicial[FILE_NAME_LEN];
    sprintf(nomeSvgInicial, "%s.svg", nomeBaseGeo);
    char *caminhoSvgInicial = montaCaminhoCompleto(dirSaida, nomeSvgInicial);
    
    FILE* svgInicial = inicializaSVG(caminhoSvgInicial, 800, 600);
    if (svgInicial != NULL) {
        for (int i = 0; i < numSegmentos; i++) {
            desenhaSegmentoSVG(svgInicial, segmentos[i], "black", 1.5);
        }
        finalizaSVG(svgInicial);
        printf("SVG inicial gerado: %s\n", caminhoSvgInicial);
    }
    free(caminhoSvgInicial);
    
    // Processa arquivo .qry (se fornecido)
    if (arqQry[0] != '\0') {
        char nomeBaseQry[FILE_NAME_LEN];
        getNomeBase(arqQry, nomeBaseQry, FILE_NAME_LEN);
        
        char nomeSaidaBase[FILE_NAME_LEN];
        sprintf(nomeSaidaBase, "%s-%s", nomeBaseGeo, nomeBaseQry);
        
        char nomeTxt[FILE_NAME_LEN];
        sprintf(nomeTxt, "%s.txt", nomeSaidaBase);
        char *caminhoTxt = montaCaminhoCompleto(dirSaida, nomeTxt);
        
        char nomeSvg[FILE_NAME_LEN];
        sprintf(nomeSvg, "%s", nomeSaidaBase);
        char *caminhoSvgBase = montaCaminhoCompleto(dirSaida, nomeSvg);
        
        char *caminhoCompletoQry = montaCaminhoCompleto(dirEntrada, arqQry);
        
        processaQry(caminhoCompletoQry, caminhoTxt, caminhoSvgBase, segmentos, numSegmentos);
        
        printf("Consultas processadas. Resultados em: %s\n", caminhoTxt);
        
        free(caminhoCompletoQry);
        free(caminhoTxt);
        free(caminhoSvgBase);
    }
    
    // Libera memória
    for (int i = 0; i < numSegmentos; i++) {
        destroiSegmento(segmentos[i]);
    }
    free(segmentos);
    
    printf("Processamento concluído com sucesso.\n");
    return EXIT_SUCCESS;
}