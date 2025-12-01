#include "bomba.h"
#include "formas.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "sobreposicao.h"
#include "geometria.h"
#include "svg.h"
#include "processaGeo.h"
#define PI 3.14159265358979323846

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static Forma clonaForma(Forma forma, double dx, double dy, int novoId);

void processaBombaDestruicao(double x, double y, Lista formas, FILE *arquivoTxt, const char *sufixo, const char *caminhoSaida, const char *nomeBase) {
    (void)caminhoSaida;  // ← ADICIONAR para silenciar warning
    (void)nomeBase;      // ← ADICIONAR para silenciar warning

    if (formas == NULL) {
        return;
    }
    
    if (arquivoTxt != NULL) {
        fprintf(arquivoTxt, "[*] d %.2f %.2f %s\n", x, y, sufixo);
    }
    
    Poligono regiao = calculaRegiaoVisibilidade(x, y, formas);
    
    Lista formasDestruir = criaLista();
    int numFormas = tamanhoLista(formas);
    int i;
    
    for (i = 0; i < numFormas; i++) {
        Forma f = (Forma) getListaPosicao(formas, i);
        if (f != NULL && formaNoPoligono(f, regiao)) {
            insereListaFim(formasDestruir, f);
        }
    }
    
    int numDestruidas = tamanhoLista(formasDestruir);
    for (i = 0; i < numDestruidas; i++) {
        Forma f = (Forma) getListaPosicao(formasDestruir, i);
        if (f != NULL) {
            int id = getFormaId(f);
            TipoForma tipo = getFormaTipo(f);
            
            if (arquivoTxt != NULL) {
                const char *nomeTipo = "";
                switch (tipo) {
                    case TIPO_CIRCULO: nomeTipo = "círculo"; break;
                    case TIPO_RETANGULO: nomeTipo = "retângulo"; break;
                    case TIPO_LINHA: nomeTipo = "linha"; break;
                    case TIPO_TEXTO: nomeTipo = "texto"; break;
                }
                fprintf(arquivoTxt, "  Destruído: %s (ID: %d)\n", nomeTipo, id);
            }
            
            removeFormaPorId(formas, id);
        }
    }
    
    if (arquivoTxt != NULL) {
        fprintf(arquivoTxt, "\n");
    }
    
    destroiLista(formasDestruir);
    destroiPoligono(regiao);
}

void processaBombaPintura(double x, double y, const char *cor, Lista formas,FILE *arquivoTxt, const char *sufixo,  const char *caminhoSaida, const char *nomeBase) {
    (void)caminhoSaida; 
    (void)nomeBase; 
    
    if (formas == NULL || cor == NULL) {
        return;
    }
    
    if (arquivoTxt != NULL) {
        fprintf(arquivoTxt, "[*] p %.2f %.2f %s %s\n", x, y, cor, sufixo);
    }
    
    Poligono regiao = calculaRegiaoVisibilidade(x, y, formas);
    
    int numFormas = tamanhoLista(formas);
    int i;
    
    for (i = 0; i < numFormas; i++) {
        Forma f = (Forma) getListaPosicao(formas, i);
        if (f != NULL && formaNoPoligono(f, regiao)) {
            int id = getFormaId(f);
            TipoForma tipo = getFormaTipo(f);
            
            setFormaCorBorda(f, cor);
            setFormaCorPreenchimento(f, cor);
            
            if (arquivoTxt != NULL) {
                const char *nomeTipo = "";
                switch (tipo) {
                    case TIPO_CIRCULO: nomeTipo = "círculo"; break;
                    case TIPO_RETANGULO: nomeTipo = "retângulo"; break;
                    case TIPO_LINHA: nomeTipo = "linha"; break;
                    case TIPO_TEXTO: nomeTipo = "texto"; break;
                }
                fprintf(arquivoTxt, "  Pintado: %s (ID: %d)\n", nomeTipo, id);
            }
        }
    }
    
    if (arquivoTxt != NULL) {
        fprintf(arquivoTxt, "\n");
    }
    
    destroiPoligono(regiao);
}

void processaBombaClonagem(double x, double y, double dx, double dy, Lista formas, Gerador gerador, FILE *arquivoTxt, const char *sufixo, const char *caminhoSaida, const char *nomeBase) {
    (void)caminhoSaida;  
    (void)nomeBase;

    if (formas == NULL || gerador == NULL) {
        return;
    }
    
    if (arquivoTxt != NULL) {
        fprintf(arquivoTxt, "[*] cln %.2f %.2f %.2f %.2f %s\n", x, y, dx, dy, sufixo);
    }
    
    Poligono regiao = calculaRegiaoVisibilidade(x, y, formas);
    
    Lista formasClonar = criaLista();
    int numFormas = tamanhoLista(formas);
    int i;
    
    for (i = 0; i < numFormas; i++) {
        Forma f = (Forma) getListaPosicao(formas, i);
        if (f != NULL && formaNoPoligono(f, regiao)) {
            insereListaFim(formasClonar, f);
        }
    }
    
    int numClones = tamanhoLista(formasClonar);

    for (i = 0; i < numClones; i++) {
        Forma original = (Forma) getListaPosicao(formasClonar, i);
        if (original != NULL) {
            int idOriginal = getFormaId(original);
            int novoId = geraProximoId(gerador);
            
            Forma clone = clonaForma(original, dx, dy, novoId);
            if (clone != NULL) {
                insereListaFim(formas, clone);
                
                if (arquivoTxt != NULL) {
                    TipoForma tipo = getFormaTipo(original);
                    const char *nomeTipo = "";
                    switch (tipo) {
                        case TIPO_CIRCULO: nomeTipo = "círculo"; break;
                        case TIPO_RETANGULO: nomeTipo = "retângulo"; break;
                        case TIPO_LINHA: nomeTipo = "linha"; break;
                        case TIPO_TEXTO: nomeTipo = "texto"; break;
                    }
                    fprintf(arquivoTxt, "  Clonado: %s (ID original: %d, ID clone: %d)\n", 
                           nomeTipo, idOriginal, novoId);
                }
            }
        }
    }
    
    if (arquivoTxt != NULL) {
        fprintf(arquivoTxt, "\n");
    }
    
    destroiLista(formasClonar);
    destroiPoligono(regiao);
}

static Forma clonaForma(Forma forma, double dx, double dy, int novoId) {
    if (forma == NULL) {
        return NULL;
    }
    
    TipoForma tipo = getFormaTipo(forma);
    void *dados = getFormaAssoc(forma);
    
    switch (tipo) {
        case TIPO_CIRCULO: {
            double x = getXCirculo(dados) + dx;
            double y = getYCirculo(dados) + dy;
            double r = getRCirculo(dados);
            char *corb = getCorbCirculo(dados);
            char *corp = getCorpCirculo(dados);
            
            Circulo c = criarCirculo(novoId, x, y, r, corb, corp, false, 0);
            return criaForma(novoId, TIPO_CIRCULO, c);
        }
        
        case TIPO_RETANGULO: {
            double x = getXRetangulo(dados) + dx;
            double y = getYRetangulo(dados) + dy;
            double w = getLarguraRetangulo(dados);
            double h = getAlturaRetangulo(dados);
            char *corb = getCorbRetangulo(dados);
            char *corp = getCorpRetangulo(dados);
            
            Retangulo r = criarRetangulo(novoId, x, y, w, h, corb, corp, false, 0);
            return criaForma(novoId, TIPO_RETANGULO, r);
        }
        
        case TIPO_LINHA: {
            double x1 = getX1Linha(dados) + dx;
            double y1 = getY1Linha(dados) + dy;
            double x2 = getX2Linha(dados) + dx;
            double y2 = getY2Linha(dados) + dy;
            char *cor = getCorLinha(dados);
            
            Linha l = criarLinha(novoId, x1, y1, x2, y2, cor, false, 0);
            return criaForma(novoId, TIPO_LINHA, l);
        }
        
        case TIPO_TEXTO: {
            double x = getXTexto(dados) + dx;
            double y = getYTexto(dados) + dy;
            char *corb = getCorbTexto(dados);
            char *corp = getCorpTexto(dados);
            char ancora = getAncora(dados);
            char *texto = getTexto(dados);
            Estilo estilo = getEstiloTexto(dados);
            
            Texto t = criarTexto(novoId, x, y, corb, corp, ancora, texto, estilo);
            return criaForma(novoId, TIPO_TEXTO, t);
        }
    }
    
    return NULL;
}


Poligono calculaRegiaoVisibilidade(double x, double y, Lista formas) {
    (void)formas;
    Poligono regiao = criaPoligono();
    
    double raio = 1000.0;
    int numPontos = 36;
    int i;
    
    for (i = 0; i < numPontos; i++) {
        double angulo = (2.0 * PI * i) / numPontos;
        double px = x + raio * cos(angulo);
        double py = y + raio * sin(angulo);
        
        Ponto p = criaPonto(px, py);
        insertVertice(regiao, p);
        destroiPonto(p);
    }
    
    fechaPoligono(regiao);
    
    return regiao;
}

bool formaNoPoligono(Forma forma, Poligono regiao) {
    if (forma == NULL || regiao == NULL) {
        return false;
    }
    
    TipoForma tipo = getFormaTipo(forma);
    void *dados = getFormaAssoc(forma);
    
    switch (tipo) {
        case TIPO_CIRCULO: {
            double cx = getXCirculo(dados);
            double cy = getYCirculo(dados);
            double r = getRCirculo(dados);
            
            Ponto centro = criaPonto(cx, cy);
            bool resultado = pontoNoPoligono(regiao, centro);
            destroiPonto(centro);
            
            if (resultado) return true;
            
            Lista segmentos = getSegmentos(regiao);
            int numSeg = tamanhoLista(segmentos);
            int i;
            for (i = 0; i < numSeg; i++) {
                Segmento s = (Segmento) getListaPosicao(segmentos, i);
                if (distanciaPontoAoSegmento(centro, s) <= r) {
                    destroiPonto(centro);
                    return true;
                }
            }
            
            return false;
        }
        
        case TIPO_RETANGULO: {
            double rx = getXRetangulo(dados);
            double ry = getYRetangulo(dados);
            double w = getLarguraRetangulo(dados);
            double h = getAlturaRetangulo(dados);
            
            Ponto vertices[4];
            vertices[0] = criaPonto(rx, ry);
            vertices[1] = criaPonto(rx + w, ry);
            vertices[2] = criaPonto(rx + w, ry + h);
            vertices[3] = criaPonto(rx, ry + h);
            
            int i;
            for (i = 0; i < 4; i++) {
                if (pontoNoPoligono(regiao, vertices[i])) {
                    for (int j = 0; j < 4; j++) destroiPonto(vertices[j]);
                    return true;
                }
            }
            
            for (i = 0; i < 4; i++) destroiPonto(vertices[i]);
            return false;
        }
        
        case TIPO_LINHA: {
            double x1 = getX1Linha(dados);
            double y1 = getY1Linha(dados);
            double x2 = getX2Linha(dados);
            double y2 = getY2Linha(dados);
            
            Ponto p1 = criaPonto(x1, y1);
            Ponto p2 = criaPonto(x2, y2);
            
            bool resultado = pontoNoPoligono(regiao, p1) || pontoNoPoligono(regiao, p2);
            
            destroiPonto(p1);
            destroiPonto(p2);
            
            return resultado;
        }
        
        case TIPO_TEXTO: {
            double x1, y1, x2, y2;
            converterTextoParaLinha(dados, &x1, &y1, &x2, &y2);
            
            Ponto p1 = criaPonto(x1, y1);
            Ponto p2 = criaPonto(x2, y2);
            
            bool resultado = pontoNoPoligono(regiao, p1) || pontoNoPoligono(regiao, p2);
            
            destroiPonto(p1);
            destroiPonto(p2);
            
            return resultado;
        }
    }
    
    return false;
}
