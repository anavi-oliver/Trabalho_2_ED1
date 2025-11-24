#include "gerador.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct gerador {
    int proximoId;
} GeradorStruct;

Gerador criaGerador(int idInicial) {
    if (idInicial < 0) {
        fprintf(stderr, "Aviso: ID inicial negativo, usando 0.\n");
        idInicial = 0;
    }
    
    GeradorStruct *g = (GeradorStruct*) malloc(sizeof(GeradorStruct));
    if (g == NULL) {
        fprintf(stderr, "Erro: falha ao alocar gerador de IDs.\n");
        return NULL;
    }
    
    g->proximoId = idInicial;
    
    return (Gerador) g;
}

void destroiGerador(Gerador g) {
    if (g == NULL) {
        return;
    }
    free(g);
}

int geraProximoId(Gerador g) {
    if (g == NULL) {
        return -1;
    }
    
    GeradorStruct *gen = (GeradorStruct*) g;
    int id = gen->proximoId;
    gen->proximoId++;
    
    return id;
}

int consultaProximoId(Gerador g) {
    if (g == NULL) {
        return -1;
    }
    
    GeradorStruct *gen = (GeradorStruct*) g;
    return gen->proximoId;
}

void setProximoId(Gerador g, int proximoId) {
    if (g == NULL) {
        return;
    }
    
    if (proximoId < 0) {
        fprintf(stderr, "Aviso: tentativa de setar ID negativo ignorada.\n");
        return;
    }
    
    GeradorStruct *gen = (GeradorStruct*) g;
    gen->proximoId = proximoId;
}

int reservaIds(Gerador g, int quantidade) {
    if (g == NULL || quantidade <= 0) {
        return -1;
    }
    
    GeradorStruct *gen = (GeradorStruct*) g;
    int primeiroId = gen->proximoId;
    gen->proximoId += quantidade;
    
    return primeiroId;
}