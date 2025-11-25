#include "geometria.h"
#define PI 3.14159265358979323846

#include <math.h>
#include <stdlib.h>

#define EPSILON 1e-10

/*                    FUNÇÕES DE ORIENTAÇÃO                    */

double orientacao(Ponto p, Ponto q, Ponto r) {
    if (p == NULL || q == NULL || r == NULL) {
        return 0.0;
    }
    
    double px = getXPonto(p);
    double py = getYPonto(p);
    double qx = getXPonto(q);
    double qy = getYPonto(q);
    double rx = getXPonto(r);
    double ry = getYPonto(r);
    
    double valor = (qy - py) * (rx - qx) - (qx - px) * (ry - qy);
    
    if (fabs(valor) < EPSILON) {
        return 0.0;
    }
    
    return valor;
}

int direcaoOrientacao(Ponto p, Ponto q, Ponto r) {
    double val = orientacao(p, q, r);
    
    if (fabs(val) < EPSILON) {
        return 0;
    }
    
    return (val > 0) ? 1 : -1;
}

/*                    FUNÇÕES DE INTERSEÇÃO                    */

bool segmentosIntersectam(Segmento s1, Segmento s2) {
    if (s1 == NULL || s2 == NULL) {
        return false;
    }
    
    Ponto p1 = getPonto1Segmento(s1);
    Ponto q1 = getPonto2Segmento(s1);
    Ponto p2 = getPonto1Segmento(s2);
    Ponto q2 = getPonto2Segmento(s2);
    
    int o1 = direcaoOrientacao(p1, q1, p2);
    int o2 = direcaoOrientacao(p1, q1, q2);
    int o3 = direcaoOrientacao(p2, q2, p1);
    int o4 = direcaoOrientacao(p2, q2, q1);
    
    if (o1 != o2 && o3 != o4) {
        return true;
    }
    
    if (o1 == 0 && pontoNoSegmentoPQR(p1, p2, q1)) return true;
    if (o2 == 0 && pontoNoSegmentoPQR(p1, q2, q1)) return true;
    if (o3 == 0 && pontoNoSegmentoPQR(p2, p1, q2)) return true;
    if (o4 == 0 && pontoNoSegmentoPQR(p2, q1, q2)) return true;
    
    return false;
}

Ponto intersecaoSegmentos(Segmento s1, Segmento s2) {
    if (s1 == NULL || s2 == NULL) {
        return NULL;
    }
    
    Ponto p1 = getPonto1Segmento(s1);
    Ponto q1 = getPonto2Segmento(s1);
    Ponto p2 = getPonto1Segmento(s2);
    Ponto q2 = getPonto2Segmento(s2);
    
    double x1 = getXPonto(p1);
    double y1 = getYPonto(p1);
    double x2 = getXPonto(q1);
    double y2 = getYPonto(q1);
    double x3 = getXPonto(p2);
    double y3 = getYPonto(p2);
    double x4 = getXPonto(q2);
    double y4 = getYPonto(q2);
    
    double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    
    if (fabs(denom) < EPSILON) {
        return NULL;
    }
    
    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
    
    double ix = x1 + t * (x2 - x1);
    double iy = y1 + t * (y2 - y1);
    
    return criaPonto(ix, iy);
}

/*                    FUNÇÕES DE DISTÂNCIA                    */

double distanciaPontoAoSegmento(Ponto p, Segmento s) {
    if (p == NULL || s == NULL) {
        return 0.0;
    }
    
    Ponto p1 = getPonto1Segmento(s);
    Ponto p2 = getPonto2Segmento(s);
    
    double px = getXPonto(p);
    double py = getYPonto(p);
    double x1 = getXPonto(p1);
    double y1 = getYPonto(p1);
    double x2 = getXPonto(p2);
    double y2 = getYPonto(p2);
    
    double dx = x2 - x1;
    double dy = y2 - y1;
    double comprimentoQuad = dx * dx + dy * dy;
    
    if (comprimentoQuad < EPSILON) {
        return distanciaPontos(p, p1);
    }
    
    double t = ((px - x1) * dx + (py - y1) * dy) / comprimentoQuad;
    
    if (t < 0.0) {
        return distanciaPontos(p, p1);
    }
    
    if (t > 1.0) {
        return distanciaPontos(p, p2);
    }
    
    Ponto projecao = criaPonto(x1 + t * dx, y1 + t * dy);
    double dist = distanciaPontos(p, projecao);
    destroiPonto(projecao);
    
    return dist;
}

/*                    FUNÇÕES AUXILIARES                    */

bool pontoNoSegmentoPQR(Ponto p, Ponto q, Ponto r) {
    if (p == NULL || q == NULL || r == NULL) {
        return false;
    }
    
    double px = getXPonto(p);
    double py = getYPonto(p);
    double qx = getXPonto(q);
    double qy = getYPonto(q);
    double rx = getXPonto(r);
    double ry = getYPonto(r);
    
    double minX = (px < rx) ? px : rx;
    double maxX = (px > rx) ? px : rx;
    double minY = (py < ry) ? py : ry;
    double maxY = (py > ry) ? py : ry;
    
    return (qx >= minX && qx <= maxX && qy >= minY && qy <= maxY);
}

double areaTriangulo(Ponto p1, Ponto p2, Ponto p3) {
    if (p1 == NULL || p2 == NULL || p3 == NULL) {
        return 0.0;
    }
    
    double x1 = getXPonto(p1);
    double y1 = getYPonto(p1);
    double x2 = getXPonto(p2);
    double y2 = getYPonto(p2);
    double x3 = getXPonto(p3);
    double y3 = getYPonto(p3);
    
    double area = fabs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
    
    return area;
}

double anguloEntreSegmentos(Segmento s1, Segmento s2) {
    if (s1 == NULL || s2 == NULL) {
        return 0.0;
    }
    
    Ponto p1 = getPonto1Segmento(s1);
    Ponto q1 = getPonto2Segmento(s1);
    Ponto p2 = getPonto1Segmento(s2);
    Ponto q2 = getPonto2Segmento(s2);
    
    double dx1 = getXPonto(q1) - getXPonto(p1);
    double dy1 = getYPonto(q1) - getYPonto(p1);
    double dx2 = getXPonto(q2) - getXPonto(p2);
    double dy2 = getYPonto(q2) - getYPonto(p2);
    
    double angulo1 = atan2(dy1, dx1);
    double angulo2 = atan2(dy2, dx2);
    
    double diferenca = angulo2 - angulo1;
    
    while (diferenca < 0) diferenca += 2 * PI;
    while (diferenca >= 2 * PI) diferenca -= 2 * PI;
    
    return diferenca;
}