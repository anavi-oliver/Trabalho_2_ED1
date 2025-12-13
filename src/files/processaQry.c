#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "processaQry.h"
#include "visibilidade.h" 
#include "formas.h"       
#include "lista.h"        
#include "gerador.h"
#include "svg.h"          // Necessário para escreveFormasSVG e outras funcoes de desenho

// Includes das formas específicas para pegar coordenadas
#include "linha.h"
#include "retangulo.h"
#include "circulo.h"
#include "texto.h"
#include "anteparo.h" 

#ifndef PI
#define PI 3.14159265358979323846
#endif

// --- FUNÇÕES AUXILIARES DE GEOMETRIA ---

bool pontoInternoVis(double x, double y, Lista poligonoVis) {
    bool inside = false;
    int n = tamanhoLista(poligonoVis);
    if (n < 3) return false;

    double* VX = malloc(n * sizeof(double));
    double* VY = malloc(n * sizeof(double));

    for(int i=0; i<n; i++) {
        PontoVis p = getListaPosicao(poligonoVis, i);
        if (p) {
            VX[i] = getPontoVisX(p); 
            VY[i] = getPontoVisY(p); 
        }
    }

    for (int i = 0, j = n - 1; i < n; j = i++) {
        if (((VY[i] > y) != (VY[j] > y)) &&
            (x < (VX[j] - VX[i]) * (y - VY[i]) / (VY[j] - VY[i]) + VX[i])) {
            inside = !inside;
        }
    }

    free(VX);
    free(VY);
    return inside;
}

// Verifica se uma FORMA está na região de visibilidade
bool formaNoPoligonoVis(Forma f, Lista regiao) {
    if (!f || !regiao) return false;

    TipoForma tipo = getFormaTipo(f);
    void *dados = getFormaAssoc(f);

    switch (tipo) {
        case TIPO_CIRCULO: {
            double cx = getXCirculo(dados); 
            double cy = getYCirculo(dados); 
            return pontoInternoVis(cx, cy, regiao);
        }
        case TIPO_RETANGULO: {
            double rx = getXRetangulo(dados);       
            double ry = getYRetangulo(dados);       
            double w = getLarguraRetangulo(dados);  
            double h = getAlturaRetangulo(dados);   
            
            // Testa os 4 cantos
            if (pontoInternoVis(rx, ry, regiao)) return true;
            if (pontoInternoVis(rx+w, ry, regiao)) return true;
            if (pontoInternoVis(rx+w, ry+h, regiao)) return true;
            if (pontoInternoVis(rx, ry+h, regiao)) return true;
            return false;
        }
        case TIPO_LINHA: {
            double x1 = getX1Linha(dados); 
            double y1 = getY1Linha(dados); 
            double x2 = getX2Linha(dados); 
            double y2 = getY2Linha(dados); 
            return pontoInternoVis(x1, y1, regiao) || pontoInternoVis(x2, y2, regiao);
        }
        case TIPO_TEXTO: {
            double x = getXTexto(dados);  
            double y = getYTexto(dados); 
            return pontoInternoVis(x, y, regiao);
        }
        default: return false;
    }
}

// --- CLONAGEM ---

Forma clonaForma(Forma forma, double dx, double dy, int novoId) {
    if (forma == NULL) return NULL;
    
    TipoForma tipo = getFormaTipo(forma);
    void *dados = getFormaAssoc(forma);
    
    switch (tipo) {
        case TIPO_CIRCULO: {
            double x = getXCirculo(dados) + dx;
            double y = getYCirculo(dados) + dy;
            double r = getRCirculo(dados);
            return criaForma(novoId, TIPO_CIRCULO, criarCirculo(novoId, x, y, r, "black", "red", false, 0));
        }
        case TIPO_RETANGULO: {
            double x = getXRetangulo(dados) + dx;
            double y = getYRetangulo(dados) + dy;
            double w = getLarguraRetangulo(dados);
            double h = getAlturaRetangulo(dados);
            return criaForma(novoId, TIPO_RETANGULO, criarRetangulo(novoId, x, y, w, h, "black", "blue", false, 0));
        }
        case TIPO_LINHA: {
            double x1 = getX1Linha(dados) + dx;
            double y1 = getY1Linha(dados) + dy;
            double x2 = getX2Linha(dados) + dx;
            double y2 = getY2Linha(dados) + dy;
            return criaForma(novoId, TIPO_LINHA, criarLinha(novoId, x1, y1, x2, y2, "black", false, 0));
        }
        default: return NULL;
    }
}

// --- EFEITOS ---

void aplicarDestruicao(Lista formas, Lista poligonoVis) {
    int qtd = tamanhoLista(formas);
    for (int i = qtd - 1; i >= 0; i--) {
        Forma f = (Forma) getListaPosicao(formas, i);
        if (formaNoPoligonoVis(f, poligonoVis)) {
            removeListaPosicao(formas, i);
        }
    }
}

void aplicarPintura(Lista formas, Lista poligonoVis, char* cor) {
    int qtd = tamanhoLista(formas);
    for (int i = 0; i < qtd; i++) {
        Forma f = (Forma) getListaPosicao(formas, i);
        if (formaNoPoligonoVis(f, poligonoVis)) {
            setFormaCorPreenchimento(f, cor);
        }
    }
}

void aplicarClonagem(Lista formas, Lista poligonoVis, double dx, double dy, Gerador gerador) {
    Lista clones = criaLista();
    int qtd = tamanhoLista(formas);
    
    for (int i = 0; i < qtd; i++) {
        Forma f = (Forma) getListaPosicao(formas, i);
        if (formaNoPoligonoVis(f, poligonoVis)) {
            insereListaFim(clones, f);
        }
    }
    
    int qtdClones = tamanhoLista(clones);
    for (int i = 0; i < qtdClones; i++) {
        Forma original = (Forma) getListaPosicao(clones, i);
        int novoId = geraProximoId(gerador);
        Forma clone = clonaForma(original, dx, dy, novoId);
        if (clone) {
            insereListaFim(formas, clone);
        }
    }
    destroiLista(clones);
}

void montaCaminhoFile(char* buffer, const char* dir, const char* nome) {
    if (dir[strlen(dir)-1] == '/' || dir[strlen(dir)-1] == '\\')
        sprintf(buffer, "%s%s", dir, nome);
    else
        sprintf(buffer, "%s/%s", dir, nome);
}

// --- CALCULO DE TAMANHO DO SVG ---

void obterDimensoesMaximas(Lista formas, double *wMax, double *hMax) {
    *wMax = 1000.0; // Valor minimo padrao
    *hMax = 1000.0;
    
    int qtd = tamanhoLista(formas);
    for(int i=0; i<qtd; i++) {
        Forma f = getListaPosicao(formas, i);
        TipoForma t = getFormaTipo(f);
        void* d = getFormaAssoc(f);
        double maxx = 0, maxy = 0;
        
        if(t == TIPO_RETANGULO) {
            maxx = getXRetangulo(d) + getLarguraRetangulo(d);
            maxy = getYRetangulo(d) + getAlturaRetangulo(d);
        } else if (t == TIPO_CIRCULO) {
            maxx = getXCirculo(d) + getRCirculo(d);
            maxy = getYCirculo(d) + getRCirculo(d);
        } else if (t == TIPO_LINHA) {
            maxx = (getX1Linha(d) > getX2Linha(d)) ? getX1Linha(d) : getX2Linha(d);
            maxy = (getY1Linha(d) > getY2Linha(d)) ? getY1Linha(d) : getY2Linha(d);
        }
        
        if(maxx > *wMax) *wMax = maxx;
        if(maxy > *hMax) *hMax = maxy;
    }
    
    // Margem de segurança
    *wMax += 50.0;
    *hMax += 50.0;
}

// --- FUNÇÃO PRINCIPAL ---

void processaArquivoQry(const char* entrada, Lista formas, Gerador gerador, const char* dirSaida, const char* nomeBase, char tipoSort, int threshold) {
    FILE* qry = fopen(entrada, "r");
    if (!qry) {
        printf("ERRO: Nao abriu QRY: %s\n", entrada);
        return;
    }

    // Calcula dimensoes para o SVG
    double maxW, maxH;
    obterDimensoesMaximas(formas, &maxW, &maxH);

    char linha[512];

    while (fgets(linha, sizeof(linha), qry)) {
        char comando[10];
        if (sscanf(linha, "%s", comando) != 1) continue;

        // === d: DESTRUIÇÃO ===
        if (strcmp(comando, "d") == 0) {
            double bx, by;
            char sufixo[256] = "";
            sscanf(linha, "d %lf %lf %s", &bx, &by, sufixo);
            
            Lista poli = calcular_visibilidade(bx, by, formas, tipoSort, threshold);
            
            char nomeArq[1024], pathSvg[1024];
            sprintf(nomeArq, "%s-d-%s.svg", nomeBase, (strlen(sufixo)>0)?sufixo:"idx");
            montaCaminhoFile(pathSvg, dirSaida, nomeArq);
            
            FILE* svg = fopen(pathSvg, "w");
            if(svg) {
                // Cabecalho e fundo
                fprintf(svg, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%.2f\" height=\"%.2f\">\n", maxW, maxH);
                // Borda transparente (fill=none)
                fprintf(svg, "\t<rect x=\"0\" y=\"0\" width=\"%.2f\" height=\"%.2f\" fill=\"none\" stroke=\"black\" stroke-width=\"1\" />\n", maxW, maxH);
                
                // 1. DESENHA O CENÁRIO (AS FORMAS ANTES)
                escreveFormasSVG(svg, formas);
                
                // 2. DESENHA A VISIBILIDADE (COM OPACIDADE PARA VER O QUE TEM ATRAS, SE QUISER)
                // "red" para destruicao
                desenhar_poligono_visibilidade(svg, poli, "red");
                
                // 3. DESENHA O PONTO DE TIRO
                fprintf(svg, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"5\" fill=\"red\" stroke=\"black\" stroke-width=\"1\"/>\n", bx, by);
                
                fprintf(svg, "</svg>");
                fclose(svg);
            }
            
            aplicarDestruicao(formas, poli);
            destruir_lista_pontos(poli);
        }
        
        // === p: PINTURA ===
        else if (strcmp(comando, "p") == 0) {
            double bx, by;
            char cor[128], sufixo[256] = "";
            sscanf(linha, "p %lf %lf %s %s", &bx, &by, cor, sufixo);
            
            Lista poli = calcular_visibilidade(bx, by, formas, tipoSort, threshold);
            
            char nomeArq[1024], pathSvg[1024];
            sprintf(nomeArq, "%s-p-%s.svg", nomeBase, (strlen(sufixo)>0)?sufixo:"idx");
            montaCaminhoFile(pathSvg, dirSaida, nomeArq);
            
            FILE* svg = fopen(pathSvg, "w");
            if(svg) {
                fprintf(svg, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%.2f\" height=\"%.2f\">\n", maxW, maxH);
                fprintf(svg, "\t<rect x=\"0\" y=\"0\" width=\"%.2f\" height=\"%.2f\" fill=\"none\" stroke=\"black\" stroke-width=\"1\" />\n", maxW, maxH);
                
                escreveFormasSVG(svg, formas); // Cenário
                
                desenhar_poligono_visibilidade(svg, poli, cor);
                fprintf(svg, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"5\" fill=\"%s\" stroke=\"black\"/>\n", bx, by, cor);
                
                fprintf(svg, "</svg>");
                fclose(svg);
            }
            
            aplicarPintura(formas, poli, cor);
            destruir_lista_pontos(poli);
        }

        // === cln: CLONAGEM ===
        else if (strcmp(comando, "cln") == 0) {
            double bx, by, dx, dy;
            char sufixo[256] = "";
            sscanf(linha, "cln %lf %lf %lf %lf %s", &bx, &by, &dx, &dy, sufixo);
            
            Lista poli = calcular_visibilidade(bx, by, formas, tipoSort, threshold);
            
            char nomeArq[1024], pathSvg[1024];
            sprintf(nomeArq, "%s-cln-%s.svg", nomeBase, (strlen(sufixo)>0)?sufixo:"idx");
            montaCaminhoFile(pathSvg, dirSaida, nomeArq);
            
            FILE* svg = fopen(pathSvg, "w");
            if(svg) {
                fprintf(svg, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%.2f\" height=\"%.2f\">\n", maxW, maxH);
                fprintf(svg, "\t<rect x=\"0\" y=\"0\" width=\"%.2f\" height=\"%.2f\" fill=\"none\" stroke=\"black\" stroke-width=\"1\" />\n", maxW, maxH);
                
                escreveFormasSVG(svg, formas); // Cenário
                
                desenhar_poligono_visibilidade(svg, poli, "blue"); // Clone usa azul
                fprintf(svg, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"5\" fill=\"blue\" stroke=\"black\"/>\n", bx, by);
                
                fprintf(svg, "</svg>");
                fclose(svg);
            }
            
            aplicarClonagem(formas, poli, dx, dy, gerador);
            destruir_lista_pontos(poli);
        }

        // === a: ANTEPARO ===
        else if (strcmp(comando, "a") == 0) {
             int id;
             char orientacao[10] = ""; 
             int lidos = sscanf(linha, "a %d %s", &id, orientacao);
             
             if (lidos >= 1) {
                char ori = (strlen(orientacao) > 0) ? orientacao[0] : 'i';
                
                int qtd = tamanhoLista(formas);
                for(int i=0; i<qtd; i++) {
                    Forma f = (Forma)getListaPosicao(formas, i);
                    if(f && getFormaId(f) == id) {
                        transformaEmAnteparo(f, ori, gerador, formas);
                        break;
                    }
                }
             }
        }
    }
    fclose(qry);
}