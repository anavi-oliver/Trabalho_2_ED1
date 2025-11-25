#include "anteparo.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "processaGeo.h"
#include <stdio.h>
#include <string.h>

void transformaEmAnteparo(Forma forma, char orientacao, Gerador gerador, Lista formas) {
    if (forma == NULL || gerador == NULL || formas == NULL) {
        return;
    }
    
    TipoForma tipo = getFormaTipo(forma);
    void *dadosEspecificos = getFormaAssoc(forma);
    
    switch (tipo) {
        case TIPO_CIRCULO: {
            int idNovo = geraProximoId(gerador);
            circuloParaAnteparo(dadosEspecificos, orientacao, idNovo, formas);
            break;
        }
        
        case TIPO_RETANGULO:
            retanguloParaAnteparo(dadosEspecificos, gerador, formas);
            break;
        
        case TIPO_TEXTO: {
            int idNovo = geraProximoId(gerador);
            textoParaAnteparo(dadosEspecificos, idNovo, formas);
            break;
        }
        
        case TIPO_LINHA:
            setDispLinha(dadosEspecificos, true);
            break;
    }
}

void circuloParaAnteparo(void *c, char orientacao, int idSegmento, Lista formas) {
    if (c == NULL || formas == NULL) {
        return;
    }
    
    double cx = getXCirculo(c);
    double cy = getYCirculo(c);
    double r = getRCirculo(c);
    char *corBorda = getCorbCirculo(c);
    
    Linha segmento;
    
    if (orientacao == 'h' || orientacao == 'H') {
        segmento = criarLinha(idSegmento, cx - r, cy, cx + r, cy, corBorda, true, 0);
    } else {
        segmento = criarLinha(idSegmento, cx, cy - r, cx, cy + r, corBorda, true, 0);
    }
    
    if (segmento != NULL) {
        Forma f = criaForma(idSegmento, TIPO_LINHA, segmento);
        if (f != NULL) {
            insereListaFim(formas, f);
        }
    }
}

void retanguloParaAnteparo(void *r, Gerador gerador, Lista formas) {
    if (r == NULL || gerador == NULL || formas == NULL) {
        return;
    }
    
    double x = getXRetangulo(r);
    double y = getYRetangulo(r);
    double w = getLarguraRetangulo(r);
    double h = getAlturaRetangulo(r);
    char *corBorda = getCorbRetangulo(r);
    
    int id1 = geraProximoId(gerador);
    Linha topo = criarLinha(id1, x, y, x + w, y, corBorda, true, 0);
    
    int id2 = geraProximoId(gerador);
    Linha direita = criarLinha(id2, x + w, y, x + w, y + h, corBorda, true, 0);
    
    int id3 = geraProximoId(gerador);
    Linha baixo = criarLinha(id3, x + w, y + h, x, y + h, corBorda, true, 0);
    
    int id4 = geraProximoId(gerador);
    Linha esquerda = criarLinha(id4, x, y + h, x, y, corBorda, true, 0);
    
    if (topo) insereListaFim(formas, criaForma(id1, TIPO_LINHA, topo));
    if (direita) insereListaFim(formas, criaForma(id2, TIPO_LINHA, direita));
    if (baixo) insereListaFim(formas, criaForma(id3, TIPO_LINHA, baixo));
    if (esquerda) insereListaFim(formas, criaForma(id4, TIPO_LINHA, esquerda));
}

void textoParaAnteparo(void *t, int idSegmento, Lista formas) {
    if (t == NULL || formas == NULL) {
        return;
    }
    
    double x1, y1, x2, y2;
    converterTextoParaLinha(t, &x1, &y1, &x2, &y2);
    
    char *corBorda = getCorbTexto(t);
    
    Linha segmento = criarLinha(idSegmento, x1, y1, x2, y2, corBorda, true, 0);
    
    if (segmento != NULL) {
        Forma f = criaForma(idSegmento, TIPO_LINHA, segmento);
        if (f != NULL) {
            insereListaFim(formas, f);
        }
    }
}

void processaComandoAnteparo(Lista formas, int idInicio, int idFim,
                             char orientacao, Gerador gerador, FILE *arquivoTxt) {
    if (formas == NULL || gerador == NULL) {
        return;
    }
    
    if (arquivoTxt != NULL) {
        fprintf(arquivoTxt, "[*] a %d %d %c\n", idInicio, idFim, orientacao);
    }
    
    Lista formasATransformar = criaLista();
    
    int numFormas = tamanhoLista(formas);
    int i;
    
    for (i = 0; i < numFormas; i++) {
        Forma f = (Forma) getListaPosicao(formas, i);
        if (f != NULL) {
            int id = getFormaId(f);
            if (id >= idInicio && id <= idFim) {
                insereListaFim(formasATransformar, f);
            }
        }
    }
    
    int numTransformar = tamanhoLista(formasATransformar);
    for (i = 0; i < numTransformar; i++) {
        Forma f = (Forma) getListaPosicao(formasATransformar, i);
        if (f != NULL) {
            int idOriginal = getFormaId(f);
            TipoForma tipo = getFormaTipo(f);
            
            if (arquivoTxt != NULL) {
                const char *nomeTipo = "";
                switch (tipo) {
                    case TIPO_CIRCULO: nomeTipo = "círculo"; break;
                    case TIPO_RETANGULO: nomeTipo = "retângulo"; break;
                    case TIPO_LINHA: nomeTipo = "linha"; break;
                    case TIPO_TEXTO: nomeTipo = "texto"; break;
                }
                fprintf(arquivoTxt, "  Transformando %s (ID: %d)\n", nomeTipo, idOriginal);
            }
            
            transformaEmAnteparo(f, orientacao, gerador, formas);
            
            if (tipo != TIPO_LINHA) {
                removeFormaPorId(formas, idOriginal);
            }
        }
    }
    
    if (arquivoTxt != NULL) {
        fprintf(arquivoTxt, "\n");
    }
    
    destroiLista(formasATransformar);
}