#include <stdbool.h>
#include <stdio.h>

//fazer antes:
#include "ponto.h" 
#include "segmento.h"




double orientacao(Ponto p, Ponto q, Ponto r);
bool segmentosIntersectam(Segmento s1, Segmento s2);
Ponto intersecaoSegmentos(Segmento s1, Segmento s2);
double distanciaPontoSegmento(Ponto p, Segmento s);