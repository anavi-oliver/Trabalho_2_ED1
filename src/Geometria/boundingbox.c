#include "boundingbox.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

typedef struct boundingbox {
    double xMin;
    double yMin;
    double xMax;
    double yMax;
} BoundingBoxStruct;

BoundingBox criaBoundingBox(double xMin, double yMin, double xMax, double yMax) {
    BoundingBoxStruct *bb = (BoundingBoxStruct*) malloc(sizeof(BoundingBoxStruct));
    if (bb == NULL) {
        fprintf(stderr, "Erro: falha na alocação de memória para bounding box.\n");
        return NULL;
    }
    
    bb->xMin = xMin;
    bb->yMin = yMin;
    bb->xMax = xMax;
    bb->yMax = yMax;
    
    return (BoundingBox) bb;
}

BoundingBox criaBoundingBoxVazia() {
    return criaBoundingBox(DBL_MAX, DBL_MAX, -DBL_MAX, -DBL_MAX);
}

void destroiBoundingBox(BoundingBox bb) {
    if (bb == NULL) {
        return;
    }
    free(bb);
}

double getXMinBB(BoundingBox bb) {
    if (bb == NULL) {
        return 0.0;
    }
    BoundingBoxStruct *box = (BoundingBoxStruct*) bb;
    return box->xMin;
}

double getYMinBB(BoundingBox bb) {
    if (bb == NULL) {
        return 0.0;
    }
    BoundingBoxStruct *box = (BoundingBoxStruct*) bb;
    return box->yMin;
}

double getXMaxBB(BoundingBox bb) {
    if (bb == NULL) {
        return 0.0;
    }
    BoundingBoxStruct *box = (BoundingBoxStruct*) bb;
    return box->xMax;
}

double getYMaxBB(BoundingBox bb) {
    if (bb == NULL) {
        return 0.0;
    }
    BoundingBoxStruct *box = (BoundingBoxStruct*) bb;
    return box->yMax;
}

void expandeBBComPonto(BoundingBox bb, Ponto p) {
    if (bb == NULL || p == NULL) {
        return;
    }
    
    BoundingBoxStruct *box = (BoundingBoxStruct*) bb;
    double x = getXPonto(p);
    double y = getYPonto(p);
    
    if (x < box->xMin) box->xMin = x;
    if (x > box->xMax) box->xMax = x;
    if (y < box->yMin) box->yMin = y;
    if (y > box->yMax) box->yMax = y;
}

void expandeBBComBB(BoundingBox bb, BoundingBox outra) {
    if (bb == NULL || outra == NULL) {
        return;
    }
    
    BoundingBoxStruct *box1 = (BoundingBoxStruct*) bb;
    BoundingBoxStruct *box2 = (BoundingBoxStruct*) outra;
    
    if (box2->xMin < box1->xMin) box1->xMin = box2->xMin;
    if (box2->xMax > box1->xMax) box1->xMax = box2->xMax;
    if (box2->yMin < box1->yMin) box1->yMin = box2->yMin;
    if (box2->yMax > box1->yMax) box1->yMax = box2->yMax;
}

bool pontoNaBB(BoundingBox bb, Ponto p) {
    if (bb == NULL || p == NULL) {
        return false;
    }
    
    BoundingBoxStruct *box = (BoundingBoxStruct*) bb;
    double x = getXPonto(p);
    double y = getYPonto(p);
    
    return (x >= box->xMin && x <= box->xMax && 
            y >= box->yMin && y <= box->yMax);
}

bool BBsIntersectam(BoundingBox bb1, BoundingBox bb2) {
    if (bb1 == NULL || bb2 == NULL) {
        return false;
    }
    
    BoundingBoxStruct *box1 = (BoundingBoxStruct*) bb1;
    BoundingBoxStruct *box2 = (BoundingBoxStruct*) bb2;
    
    return !(box1->xMax < box2->xMin || 
             box2->xMax < box1->xMin || 
             box1->yMax < box2->yMin || 
             box2->yMax < box1->yMin);
}

double areaBB(BoundingBox bb) {
    if (bb == NULL) {
        return 0.0;
    }
    
    BoundingBoxStruct *box = (BoundingBoxStruct*) bb;
    double largura = box->xMax - box->xMin;
    double altura = box->yMax - box->yMin;
    
    if (largura < 0 || altura < 0) {
        return 0.0;
    }
    
    return largura * altura;
}

double larguraBB(BoundingBox bb) {
    if (bb == NULL) {
        return 0.0;
    }
    
    BoundingBoxStruct *box = (BoundingBoxStruct*) bb;
    return box->xMax - box->xMin;
}

double alturaBB(BoundingBox bb) {
    if (bb == NULL) {
        return 0.0;
    }
    
    BoundingBoxStruct *box = (BoundingBoxStruct*) bb;
    return box->yMax - box->yMin;
}