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

// --- FUNÇÃO DE RELATÓRIO (LOG) ---
void relatarForma(FILE* txt, Forma f, char* titulo) {
    if (!txt || !f) return;

    if (titulo) fprintf(txt, "%s\n", titulo);

    TipoForma tipo = getFormaTipo(f);
    void *dados = getFormaAssoc(f);
    int id = getFormaId(f);

    switch (tipo) {
        case TIPO_RETANGULO:
            fprintf(txt, "- Retangulo - id: %d\n", id);
            fprintf(txt, "\tx: %f\n", getXRetangulo((Retangulo)dados));
            fprintf(txt, "\ty: %f\n", getYRetangulo((Retangulo)dados));
            fprintf(txt, "\tlargura: %f\n", getLarguraRetangulo((Retangulo)dados));
            fprintf(txt, "\taltura: %f\n", getAlturaRetangulo((Retangulo)dados));
            // CORRIGIDO AQUI: getCorp e getCorb
            fprintf(txt, "\tcor: %s\n", getCorpRetangulo((Retangulo)dados));
            fprintf(txt, "\tcor de borda: %s\n", getCorbRetangulo((Retangulo)dados));
            break;

        case TIPO_CIRCULO:
            fprintf(txt, "- Circulo - id: %d\n", id);
            fprintf(txt, "\tx: %f\n", getXCirculo((Circulo)dados));
            fprintf(txt, "\ty: %f\n", getYCirculo((Circulo)dados));
            fprintf(txt, "\traio: %f\n", getRCirculo((Circulo)dados));
            // CORRIGIDO AQUI: getCorp e getCorb
            fprintf(txt, "\tcor: %s\n", getCorpCirculo((Circulo)dados));
            fprintf(txt, "\tcor de borda: %s\n", getCorbCirculo((Circulo)dados));
            break;

        case TIPO_LINHA:
            fprintf(txt, "- Linha - id: %d\n", id);
            fprintf(txt, "\tx1: %f\n", getX1Linha((Linha)dados));
            fprintf(txt, "\ty1: %f\n", getY1Linha((Linha)dados));
            fprintf(txt, "\tx2: %f\n", getX2Linha((Linha)dados));
            fprintf(txt, "\ty2: %f\n", getY2Linha((Linha)dados));
            fprintf(txt, "\tcor: %s\n", getCorLinha((Linha)dados));
            break;
            
        case TIPO_TEXTO:
             fprintf(txt, "- Texto - id: %d\n", id);
             fprintf(txt, "\tx: %f\n", getXTexto(dados));
             fprintf(txt, "\ty: %f\n", getYTexto(dados));
             fprintf(txt, "\tconteudo: %s\n", getTexto(dados));
             break;
    }
    fprintf(txt, "\n");
}

// --- GEOMETRIA ---

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

bool formaNoPoligonoVis(Forma f, Lista regiao) {
    if (!f || !regiao) return false;

    TipoForma tipo = getFormaTipo(f);
    void *dados = getFormaAssoc(f);

    switch (tipo) {
        case TIPO_CIRCULO: {
            double cx = getXCirculo((Circulo)dados); 
            double cy = getYCirculo((Circulo)dados); 
            return pontoInternoVis(cx, cy, regiao);
        }
        case TIPO_RETANGULO: {
            double rx = getXRetangulo((Retangulo)dados);       
            double ry = getYRetangulo((Retangulo)dados);       
            double w = getLarguraRetangulo((Retangulo)dados);  
            double h = getAlturaRetangulo((Retangulo)dados);   
            
            if (pontoInternoVis(rx, ry, regiao)) return true;
            if (pontoInternoVis(rx+w, ry, regiao)) return true;
            if (pontoInternoVis(rx+w, ry+h, regiao)) return true;
            if (pontoInternoVis(rx, ry+h, regiao)) return true;
            return false;
        }
        case TIPO_LINHA: {
            double x1 = getX1Linha((Linha)dados); 
            double y1 = getY1Linha((Linha)dados); 
            double x2 = getX2Linha((Linha)dados); 
            double y2 = getY2Linha((Linha)dados); 
            return pontoInternoVis(x1, y1, regiao) || pontoInternoVis(x2, y2, regiao);
        }
        case TIPO_TEXTO: {
            double x = getXTexto(dados);  
            double y = getYTexto(dados); 
            return pontoInternoVis(x, y, regiao);
        }
    }
    return false;
}

// --- CLONAGEM ---
Forma clonaForma(Forma forma, double dx, double dy, int novoId) {
    if (forma == NULL) return NULL;
    
    TipoForma tipo = getFormaTipo(forma);
    void *dados = getFormaAssoc(forma);
    
    switch (tipo) {
        case TIPO_CIRCULO: {
            double x = getXCirculo((Circulo)dados) + dx;
            double y = getYCirculo((Circulo)dados) + dy;
            double r = getRCirculo((Circulo)dados);
            return criaForma(novoId, TIPO_CIRCULO, criarCirculo(novoId, x, y, r, "black", "red", false, 0));
        }
        case TIPO_RETANGULO: {
            double x = getXRetangulo((Retangulo)dados) + dx;
            double y = getYRetangulo((Retangulo)dados) + dy;
            double w = getLarguraRetangulo((Retangulo)dados);
            double h = getAlturaRetangulo((Retangulo)dados);
            return criaForma(novoId, TIPO_RETANGULO, criarRetangulo(novoId, x, y, w, h, "black", "blue", false, 0));
        }
        case TIPO_LINHA: {
            double x1 = getX1Linha((Linha)dados) + dx;
            double y1 = getY1Linha((Linha)dados) + dy;
            double x2 = getX2Linha((Linha)dados) + dx;
            double y2 = getY2Linha((Linha)dados) + dy;
            return criaForma(novoId, TIPO_LINHA, criarLinha(novoId, x1, y1, x2, y2, "black", false, 0));
        }
        case TIPO_TEXTO:
        default:
            return NULL; // Texto não é clonado nesta implementação
    }
}

// --- EFEITOS ---

void aplicarDestruicao(Lista formas, Lista poligonoVis, FILE* txt) {
    int qtd = tamanhoLista(formas);
    for (int i = qtd - 1; i >= 0; i--) {
        Forma f = (Forma) getListaPosicao(formas, i);
        if (formaNoPoligonoVis(f, poligonoVis)) {
            if (txt) {
                relatarForma(txt, f, " FORMA DESTRUÍDA: ");
            }
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

void obterDimensoesMaximas(Lista formas, double *wMax, double *hMax) {
    *wMax = 1000.0; *hMax = 1000.0;
    int qtd = tamanhoLista(formas);
    for(int i=0; i<qtd; i++) {
        Forma f = getListaPosicao(formas, i);
        TipoForma t = getFormaTipo(f);
        void* d = getFormaAssoc(f);
        double maxx = 0, maxy = 0;
        
        if(t == TIPO_RETANGULO) {
            maxx = getXRetangulo((Retangulo)d) + getLarguraRetangulo((Retangulo)d);
            maxy = getYRetangulo((Retangulo)d) + getAlturaRetangulo((Retangulo)d);
        } else if (t == TIPO_CIRCULO) {
            maxx = getXCirculo((Circulo)d) + getRCirculo((Circulo)d);
            maxy = getYCirculo((Circulo)d) + getRCirculo((Circulo)d);
        } else if (t == TIPO_LINHA) {
            maxx = (getX1Linha((Linha)d) > getX2Linha((Linha)d)) ? getX1Linha((Linha)d) : getX2Linha((Linha)d);
            maxy = (getY1Linha((Linha)d) > getY2Linha((Linha)d)) ? getY1Linha((Linha)d) : getY2Linha((Linha)d);
        }
        
        if(maxx > *wMax) *wMax = maxx;
        if(maxy > *hMax) *hMax = maxy;
    }
    *wMax += 50.0; *hMax += 50.0;
}

// --- MAIN PROCESS ---

void processaArquivoQry(const char* entrada, Lista formas, Gerador gerador, const char* dirSaida, const char* nomeBase, char tipoSort, int threshold) {
    FILE* qry = fopen(entrada, "r");
    if (!qry) {
        printf("ERRO: Nao abriu QRY: %s\n", entrada);
        return;
    }

    // --- CRIAÇÃO DO ARQUIVO TXT DE RELATÓRIO ---
    char nomeTxt[1024], pathTxt[1024];
    sprintf(nomeTxt, "%s.txt", nomeBase); 
    montaCaminhoFile(pathTxt, dirSaida, nomeTxt);
    
    FILE* txtLog = fopen(pathTxt, "w"); 

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
            
            if(txtLog) fprintf(txtLog, "d %f %f %s\n\n", bx, by, sufixo);

            Lista poli = calcular_visibilidade(bx, by, formas, tipoSort, threshold);
            
            char nomeArq[1024], pathSvg[1024];
            sprintf(nomeArq, "%s-d-%s.svg", nomeBase, (strlen(sufixo)>0)?sufixo:"idx");
            montaCaminhoFile(pathSvg, dirSaida, nomeArq);
            FILE* svg = fopen(pathSvg, "w");
            if(svg) {
                fprintf(svg, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%.2f\" height=\"%.2f\">\n", maxW, maxH);
                fprintf(svg, "\t<rect x=\"0\" y=\"0\" width=\"%.2f\" height=\"%.2f\" fill=\"none\" stroke=\"black\" stroke-width=\"1\" />\n", maxW, maxH);
                escreveFormasSVG(svg, formas);
                desenhar_poligono_visibilidade(svg, poli, "red");
                fprintf(svg, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"5\" fill=\"red\" stroke=\"black\" stroke-width=\"1\"/>\n", bx, by);
                fprintf(svg, "</svg>");
                fclose(svg);
            }
            
            aplicarDestruicao(formas, poli, txtLog);
            destruir_lista_pontos(poli);
        }
        
        // === p: PINTURA ===
        else if (strcmp(comando, "p") == 0) {
            double bx, by;
            char cor[128], sufixo[256] = "";
            sscanf(linha, "p %lf %lf %s %s", &bx, &by, cor, sufixo);
            
            if(txtLog) fprintf(txtLog, "p %f %f %s %s\n\n", bx, by, cor, sufixo);

            Lista poli = calcular_visibilidade(bx, by, formas, tipoSort, threshold);
            char nomeArq[1024], pathSvg[1024];
            sprintf(nomeArq, "%s-p-%s.svg", nomeBase, (strlen(sufixo)>0)?sufixo:"idx");
            montaCaminhoFile(pathSvg, dirSaida, nomeArq);
            FILE* svg = fopen(pathSvg, "w");
            if(svg) {
                fprintf(svg, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%.2f\" height=\"%.2f\">\n", maxW, maxH);
                fprintf(svg, "\t<rect x=\"0\" y=\"0\" width=\"%.2f\" height=\"%.2f\" fill=\"none\" stroke=\"black\" stroke-width=\"1\" />\n", maxW, maxH);
                escreveFormasSVG(svg, formas);
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
            
            if(txtLog) fprintf(txtLog, "cln %f %f %f %f %s\n\n", bx, by, dx, dy, sufixo);

            Lista poli = calcular_visibilidade(bx, by, formas, tipoSort, threshold);
            char nomeArq[1024], pathSvg[1024];
            sprintf(nomeArq, "%s-cln-%s.svg", nomeBase, (strlen(sufixo)>0)?sufixo:"idx");
            montaCaminhoFile(pathSvg, dirSaida, nomeArq);
            FILE* svg = fopen(pathSvg, "w");
            if(svg) {
                fprintf(svg, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%.2f\" height=\"%.2f\">\n", maxW, maxH);
                fprintf(svg, "\t<rect x=\"0\" y=\"0\" width=\"%.2f\" height=\"%.2f\" fill=\"none\" stroke=\"black\" stroke-width=\"1\" />\n", maxW, maxH);
                escreveFormasSVG(svg, formas);
                desenhar_poligono_visibilidade(svg, poli, "blue"); 
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
                if(txtLog) fprintf(txtLog, "a %d %s\n\n", id, orientacao);

                char ori = (strlen(orientacao) > 0) ? orientacao[0] : 'i';
                int qtd = tamanhoLista(formas);
                int tamanhoAntes = qtd; 

                for(int i=0; i<qtd; i++) {
                    Forma f = (Forma)getListaPosicao(formas, i);
                    if(f && getFormaId(f) == id) {
                        
                        if (txtLog) {
                            relatarForma(txtLog, f, "- TRANSFORMAÇÃO DE FORMA EM ANTEPARO - ORIGINAL:");
                        }

                        transformaEmAnteparo(f, ori, gerador, formas);
                        
                        if (txtLog) {
                             fprintf(txtLog, "- NOVOS ANTEPAROS: \n");
                             int tamanhoDepois = tamanhoLista(formas);
                             for(int k = tamanhoAntes - 1; k < tamanhoDepois; k++) {
                                 Forma novaF = getListaPosicao(formas, k);
                                 relatarForma(txtLog, novaF, NULL);
                             }
                        }
                        break;
                    }
                }
             }
        }
    }
    
    if (txtLog) fclose(txtLog);
    fclose(qry);
}