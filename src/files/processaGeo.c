#include "processaGeo.h"
#include "formas.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA 1000

Lista processaArquivoGeo(const char *caminhoArquivo) {
    if (caminhoArquivo == NULL) {
        return NULL;
    }
    
    FILE *arquivo = fopen(caminhoArquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro: não foi possível abrir arquivo .geo: %s\n", caminhoArquivo);
        return NULL;
    }
    
    Lista formas = criaLista();
    if (formas == NULL) {
        fclose(arquivo);
        return NULL;
    }
    
    Estilo estiloAtual = criarEstilo("sans-serif", "normal", "12pt");
    char linha[MAX_LINHA];
    
    while (fgets(linha, MAX_LINHA, arquivo) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';
        
        if (strlen(linha) > 0 && linha[0] != '#') {
            processaLinha(linha, formas, estiloAtual);
        }
    }
    
    destroiEstilo(estiloAtual);
    fclose(arquivo);
    
    printf("Arquivo .geo processado: %d formas criadas\n", tamanhoLista(formas));
    
    return formas;
}

void processaLinha(char *linha, Lista formas, Estilo estiloAtual) {
    if (linha == NULL || formas == NULL) {
        return;
    }
    
    char comando;
    if (sscanf(linha, " %c", &comando) != 1) {
        return;
    }
    
    if (comando == 'c') {
        int id;
        double x, y, r;
        char corb[50], corp[50];
        
        if (sscanf(linha, "c %d %lf %lf %lf %s %s", &id, &x, &y, &r, corb, corp) == 6) {
            Circulo c = criarCirculo(id, x, y, r, corb, corp, false, 0);
            if (c != NULL) {
                Forma f = criaForma(id, TIPO_CIRCULO, c);
                if (f != NULL) {
                    insereListaFim(formas, f);
                }
            }
        }
    }
    else if (comando == 'r') {
        int id;
        double x, y, w, h;
        char corb[50], corp[50];
        
        if (sscanf(linha, "r %d %lf %lf %lf %lf %s %s", &id, &x, &y, &w, &h, corb, corp) == 7) {
            Retangulo r = criarRetangulo(id, x, y, w, h, corb, corp, false, 0);
            if (r != NULL) {
                Forma f = criaForma(id, TIPO_RETANGULO, r);
                if (f != NULL) {
                    insereListaFim(formas, f);
                }
            }
        }
    }
    else if (comando == 'l') {
        int id;
        double x1, y1, x2, y2;
        char cor[50];
        
        if (sscanf(linha, "l %d %lf %lf %lf %lf %s", &id, &x1, &y1, &x2, &y2, cor) == 6) {
            Linha l = criarLinha(id, x1, y1, x2, y2, cor, false, 0);
            if (l != NULL) {
                Forma f = criaForma(id, TIPO_LINHA, l);
                if (f != NULL) {
                    insereListaFim(formas, f);
                }
            }
        }
    }
    else if (comando == 't') {
        int id;
        double x, y;
        char corb[50], corp[50], ancora;
        char texto[500];
        
        char *p = linha;
        while (*p == ' ') p++;
        p++;
        
        if (sscanf(p, "%d %lf %lf %s %s %c ", &id, &x, &y, corb, corp, &ancora) == 6) {
            char *inicio_texto = strchr(strchr(strchr(strchr(strchr(strchr(p, ' ') + 1, ' ') + 1, ' ') + 1, ' ') + 1, ' ') + 1, ' ') + 1;
            
            if (inicio_texto != NULL) {
                strcpy(texto, inicio_texto);
                
                Texto t = criarTexto(id, x, y, corb, corp, ancora, texto, estiloAtual);
                if (t != NULL) {
                    Forma f = criaForma(id, TIPO_TEXTO, t);
                    if (f != NULL) {
                        insereListaFim(formas, f);
                    }
                }
            }
        }
    }
    else if (comando == 't' && linha[1] == 's') {
        char family[50], weight[50], size[50];
        
        if (sscanf(linha, "ts %s %s %s", family, weight, size) == 3) {
            if (strcmp(family, "sans") == 0) strcpy(family, "sans-serif");
            if (strcmp(weight, "n") == 0) strcpy(weight, "normal");
            else if (strcmp(weight, "b") == 0) strcpy(weight, "bold");
            else if (strcmp(weight, "b+") == 0) strcpy(weight, "bolder");
            else if (strcmp(weight, "l") == 0) strcpy(weight, "lighter");
            
            setFamily(estiloAtual, family);
            setWeight(estiloAtual, weight);
            setSize(estiloAtual, size);
        }
    }
}

void* buscaFormaPorId(Lista formas, int id) {
    if (formas == NULL) {
        return NULL;
    }
    
    int numFormas = tamanhoLista(formas);
    int i;
    
    for (i = 0; i < numFormas; i++) {
        Forma f = (Forma) getListaPosicao(formas, i);
        if (f != NULL && getFormaId(f) == id) {
            return f;
        }
    }
    
    return NULL;
}

bool removeFormaPorId(Lista formas, int id) {
    if (formas == NULL) {
        return false;
    }
    
    int numFormas = tamanhoLista(formas);
    int i;
    
    for (i = 0; i < numFormas; i++) {
        Forma f = (Forma) getListaPosicao(formas, i);
        if (f != NULL && getFormaId(f) == id) {
            removeListaPosicao(formas, i);
            destroiForma(f);
            return true;
        }
    }
    
    return false;
}

int calculaMaiorId(Lista formas) {
    if (formas == NULL) {
        return 0;
    }
    
    int maiorId = 0;
    int numFormas = tamanhoLista(formas);
    int i;
    
    for (i = 0; i < numFormas; i++) {
        Forma f = (Forma) getListaPosicao(formas, i);
        if (f != NULL) {
            int id = getFormaId(f);
            if (id > maiorId) {
                maiorId = id;
            }
        }
    }
    
    return maiorId;
}