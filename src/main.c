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
// --- FUNÇÕES AUXILIARES (STRING E PATHS) ---

// Retorna o nome do arquivo sem extensão e sem diretórios (ex: "dir/cidade.geo" -> "cidade")
// O retorno deve ser liberado com free()
char* obter_nome_base(const char* caminho) {
    if (!caminho) return NULL;

    // Acha a última barra (Linux ou Windows)
    const char* base = strrchr(caminho, '/');
    const char* base_win = strrchr(caminho, '\\');
    
    // Define o início do nome
    const char* inicio = caminho;
    if (base && base_win) {
        inicio = (base > base_win) ? base + 1 : base_win + 1;
    } else if (base) {
        inicio = base + 1;
    } else if (base_win) {
        inicio = base_win + 1;
    }

    // Copia para nova string
char* nome = malloc(strlen(inicio) + 1);
    if (nome) strcpy(nome, inicio);
    
    // Remove a extensão (último ponto)
    char* ponto = strrchr(nome, '.');
    if (ponto && ponto != nome) *ponto = '\0';

    return nome;
}

// Junta diretório e arquivo (ex: "entradas" + "arq.geo" -> "entradas/arq.geo")
char* monta_caminho(const char* dir, const char* arquivo) {
    if (!arquivo) return NULL;
if (!dir || strlen(dir) == 0) {
    char *copia = malloc(strlen(arquivo) + 1);
    if (copia) strcpy(copia, arquivo);
    return copia;
}

    int tam = strlen(dir) + strlen(arquivo) + 2;
    char* caminho = malloc(tam);

    // Verifica se o diretório já termina com barra
    char ultimoChar = dir[strlen(dir)-1];
    if (ultimoChar == '/' || ultimoChar == '\\') {
        sprintf(caminho, "%s%s", dir, arquivo);
    } else {
        sprintf(caminho, "%s/%s", dir, arquivo);
    }
    return caminho;
}

// Cria o nome do arquivo SVG de saída conforme regra: nomeBase + "-" + sufixo + ".svg"
char* criar_nome_saida(const char* dir_saida, const char* nome_base, const char* sufixo) {
    int tam = strlen(dir_saida) + strlen(nome_base) + 50; // +50 para segurança
    if (sufixo) tam += strlen(sufixo);
    
    char* caminho = malloc(tam);
    
    // Tratamento da barra do diretório
    char ultimoChar = (strlen(dir_saida) > 0) ? dir_saida[strlen(dir_saida)-1] : '\0';
    char *sep = (ultimoChar == '/' || ultimoChar == '\\') ? "" : "/";

    if (sufixo && strlen(sufixo) > 0) {
        // Se tiver sufixo (ex: consulta qry)
        sprintf(caminho, "%s%s%s-%s.svg", dir_saida, sep, nome_base, sufixo);
    } else {
        // Se for só o geo
        sprintf(caminho, "%s%s%s.svg", dir_saida, sep, nome_base);
    }
    
    return caminho;
}

// --- MAIN ---

int main(int argc, char *argv[]) {
    // Valores padrão
    char *dirEntrada = NULL; // Opcional (-e)
    char *dirSaida = NULL;   // Obrigatório (-o)
    char *arqGeo = NULL;     // Obrigatório (-f)
    char *arqQry = NULL;     // Opcional (-q)
    
    // Parâmetros de ordenação (Regra 1 / Problema 1)
    char tipoOrdenacao = 'q'; // Default: QuickSort
    int thresholdInsert = 10; // Default: 10
    
    // 1. Parse dos argumentos
    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-e") == 0) {
            if (i+1 < argc) dirEntrada = argv[++i];
        }
        else if (strcmp(argv[i], "-f") == 0) {
            if (i+1 < argc) arqGeo = argv[++i];
        }
        else if (strcmp(argv[i], "-o") == 0) {
            if (i+1 < argc) dirSaida = argv[++i];
        }
        else if (strcmp(argv[i], "-q") == 0) {
            if (i+1 < argc) arqQry = argv[++i];
        }
        else if (strcmp(argv[i], "-to") == 0) {
            // Tipo de ordenação (m ou q)
            if (i+1 < argc) {
                char tipo = argv[++i][0];
                if (tipo == 'm' || tipo == 'q') tipoOrdenacao = tipo;
            }
        }
        else if (strcmp(argv[i], "-in") == 0) {
            // Threshold do Insertion Sort
            if (i+1 < argc) thresholdInsert = atoi(argv[++i]);
        }
        i++;
    }

    // 2. Validação básica
    if (!arqGeo || !dirSaida) {
        fprintf(stderr, "ERRO FATAL: Argumentos -f (geo) e -o (saida) sao obrigatorios.\n");
        return EXIT_FAILURE;
    }

    // Caso o usuário não passe -e, usamos ponto atual "."
    if (!dirEntrada) dirEntrada = ".";

    printf("\n=== INICIANDO PROJETO ===\n");
    printf("Geo: %s | Qry: %s\n", arqGeo, arqQry ? arqQry : "Nao informado");
    printf("Dirs: Entrada='%s' Saida='%s'\n", dirEntrada, dirSaida);
    printf("Ordenacao: Tipo='%c' Threshold=%d\n\n", tipoOrdenacao, thresholdInsert);

    // 3. Processamento GEO
    char* pathGeoCompleto = monta_caminho(dirEntrada, arqGeo);
    char* nomeBaseGeo = obter_nome_base(arqGeo);
    
    printf("Lendo GEO: %s\n", pathGeoCompleto);
    Lista formas = processaArquivoGeo(pathGeoCompleto);
    
    if (!formas) {
        fprintf(stderr, "ERRO: Nao foi possivel ler o arquivo geo.\n");
        free(pathGeoCompleto);
        free(nomeBaseGeo);
        return EXIT_FAILURE;
    }

    // Gera SVG inicial (Apenas o .geo)
    char* pathSvgGeo = criar_nome_saida(dirSaida, nomeBaseGeo, NULL);
    printf("Gerando SVG Inicial: %s\n", pathSvgGeo);
    // Nota: Verifique se sua geraSVGCompleto aceita width/height ou ajusta automático
    geraSVGCompleto(pathSvgGeo, formas, 800, 600); 
    free(pathSvgGeo);

    // 4. Processamento QRY (se existir)
    if (arqQry) {
        char* pathQryCompleto = monta_caminho(dirEntrada, arqQry);
        char* nomeBaseQry = obter_nome_base(arqQry);
        
        // O nome base de saída para o QRY geralmente combina geo + qry
        // Ex: cidade-consulta1
        char nomeSaidaCombinado[512];
        sprintf(nomeSaidaCombinado, "%s-%s", nomeBaseGeo, nomeBaseQry);

        printf("Processando Consultas: %s\n", pathQryCompleto);
        
        // Inicializa gerador de IDs (para novos elementos criados pelo QRY)
        int maiorId = calculaMaiorId(formas);
        Gerador gerador = criaGerador(maiorId + 1);

        // OBS: Se você já implementou o Sort, você passaria 'tipoOrdenacao' e 'thresholdInsert'
        // para dentro do processaArquivoQry ou salvaria em variáveis globais/contexto.
        // Por enquanto, mantemos a chamada padrão:
processaArquivoQry(pathQryCompleto, formas, gerador, dirSaida, nomeSaidaCombinado, tipoOrdenacao, thresholdInsert);

        destroiGerador(gerador);
        free(pathQryCompleto);
        free(nomeBaseQry);
    }

    // 5. Limpeza Final
    // Assumindo que destroiListaCompleta recebe a função de destruir o dado (void*)
    destroiListaCompleta(formas, (void (*)(void*))destroiForma);
    
    free(pathGeoCompleto);
    free(nomeBaseGeo);

    printf("\n=== FIM DO PROCESSAMENTO ===\n");
    return EXIT_SUCCESS;
}