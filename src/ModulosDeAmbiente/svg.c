#include "svg.h"
#include "formas.h"

#include <stdio.h>
#include <stdlib.h>

FILE* abreSVG(const char *nomeArquivo, double largura, double altura) {
    if (nomeArquivo == NULL) {
        return NULL;
    }
    
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro: não foi possível criar arquivo SVG: %s\n", nomeArquivo);
        return NULL;
    }
    
    fprintf(arquivo, "<svg xmlns=\"http://www.w3.org/2000/svg\" ");
    fprintf(arquivo, "xmlns:xlink=\"http://www.w3.org/1999/xlink\" ");
    fprintf(arquivo, "width=\"%.2f\" height=\"%.2f\">\n", largura, altura);
    
    return arquivo;
}

void fechaSVG(FILE *arquivo) {
    if (arquivo == NULL) {
        return;
    }
    
    fprintf(arquivo, "</svg>\n");
    fclose(arquivo);
}

void escreveFormasSVG(FILE *arquivo, Lista formas) {
    if (arquivo == NULL || formas == NULL) {
        return;
    }
    
    int numFormas = tamanhoLista(formas);
    int i;
    
    for (i = 0; i < numFormas; i++) {
        Forma f = (Forma) getListaPosicao(formas, i);
        if (f != NULL) {
            desenhaForma(f, arquivo);
        }
    }
}

void desenhaPoligonoSVG(FILE *arquivo, Poligono poli, const char *cor, double opacidade) {
    if (arquivo == NULL || poli == NULL) {
        return;
    }
    
    Lista vertices = getVertices(poli);
    int numVertices = tamanhoLista(vertices);
    
    if (numVertices < 3) {
        return;
    }
    
    fprintf(arquivo, "\t<polygon points=\"");
    
    int i;
    for (i = 0; i < numVertices; i++) {
        Ponto p = (Ponto) getListaPosicao(vertices, i);
        if (p != NULL) {
            fprintf(arquivo, "%.2f,%.2f ", getXPonto(p), getYPonto(p));
        }
    }
    
    fprintf(arquivo, "\" fill=\"%s\" fill-opacity=\"%.2f\" ", cor, opacidade);
    fprintf(arquivo, "stroke=\"%s\" stroke-width=\"1\" />\n", cor);
}

void desenhaExplosaoSVG(FILE *arquivo, double x, double y, double raio, const char *cor) {
    if (arquivo == NULL) {
        return;
    }
    
    fprintf(arquivo, "\t<circle cx=\"%.2f\" cy=\"%.2f\" r=\"%.2f\" ", x, y, raio);
    fprintf(arquivo, "fill=\"%s\" fill-opacity=\"0.3\" ", cor);
    fprintf(arquivo, "stroke=\"%s\" stroke-width=\"2\" stroke-dasharray=\"5,5\" />\n", cor);
}

void desenhaTextoSVG(FILE *arquivo, double x, double y, const char *texto,  const char *cor, int tamanho) {
    if (arquivo == NULL || texto == NULL) {
        return;
    }
    
    fprintf(arquivo, "\t<text x=\"%.2f\" y=\"%.2f\" ", x, y);
    fprintf(arquivo, "font-family=\"Arial\" font-size=\"%d\" fill=\"%s\">", tamanho, cor);
    fprintf(arquivo, "%s</text>\n", texto);
}

void geraSVGCompleto(const char *nomeArquivo, Lista formas, double largura, double altura) {
    if (nomeArquivo == NULL || formas == NULL) {
        return;
    }
    
    FILE *arquivo = abreSVG(nomeArquivo, largura, altura);
    if (arquivo == NULL) {
        return;
    }
    
    escreveFormasSVG(arquivo, formas);
    fechaSVG(arquivo);
    
    printf("Arquivo SVG gerado: %s\n", nomeArquivo);
}
void desenhaSegmentoSVG(FILE *svg, Segmento s, const char *cor, double espessura) {
    if (svg == NULL || s == NULL || cor == NULL) {
        return;
    }
    
    Ponto p1 = getPonto1Segmento(s);
    Ponto p2 = getPonto2Segmento(s);
    
    double x1 = getXPonto(p1);
    double y1 = getYPonto(p1);
    double x2 = getXPonto(p2);
    double y2 = getYPonto(p2);
    
    fprintf(svg, "\t<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" "
                 "stroke=\"%s\" stroke-width=\"%.2f\" />\n",
            x1, y1, x2, y2, cor, espessura);
}

void desenhaPontoSVG(FILE *svg, double x, double y, const char *cor, double raio) {
    if (svg == NULL || cor == NULL) {
        return;
    }
    
    fprintf(svg, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"%.2f\" "
                 "fill=\"%s\" />\n",
            x, y, raio, cor);
}