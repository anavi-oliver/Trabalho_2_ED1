#ifndef PONTO_H
#define PONTO_H

#include <stdbool.h>

/*
*        TIPO ABSTRATO DE DADOS: PONTO
*
*        Este módulo define a interface para manipulação de um ponto como
*        Tipo Abstrato de Dados (TAD).  
*        A representação interna (coordenadas x, y) é escondida
*        por um ponteiro opaco (typedef void*).
*/

typedef void *Ponto;

/*                    FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO                    */

/*
Cria um novo ponto no plano cartesiano.

* x: coordenada X do ponto
* y: coordenada Y do ponto

Pré-condição: nenhuma
Pós-condição: retorna um ponteiro opaco para o ponto criado,
              ou NULL em caso de falha de alocação
*/
Ponto criaPonto(double x, double y);

/*
Libera a memória alocada para um ponto.

* p: ponteiro para o ponto a ser destruído

Pré-condição: p deve ser válido ou NULL (nesse caso não faz nada)
Pós-condição: memória do ponto é liberada
*/
void destroiPonto(Ponto p);

/*                    MÉTODOS GET (CONSULTA)                    */

/*
Retorna a coordenada X do ponto.

* p: ponteiro para o ponto

Pré-condição: p deve ser válido
Pós-condição: retorna a coordenada X
*/
double getXPonto(Ponto p);

/*
Retorna a coordenada Y do ponto.

* p: ponteiro para o ponto

Pré-condição: p deve ser válido
Pós-condição: retorna a coordenada Y
*/
double getYPonto(Ponto p);

/*                    MÉTODOS SET (MODIFICAÇÃO)                    */

/*
Define a coordenada X do ponto.

* p: ponteiro para o ponto
* x: nova coordenada X

Pré-condição: p deve ser válido
Pós-condição: coordenada X é atualizada
*/
void setXPonto(Ponto p, double x);

/*
Define a coordenada Y do ponto.

* p: ponteiro para o ponto
* y: nova coordenada Y

Pré-condição: p deve ser válido
Pós-condição: coordenada Y é atualizada
*/
void setYPonto(Ponto p, double y);

/*                    FUNÇÕES GEOMÉTRICAS                    */

/*
Calcula a distância euclidiana entre dois pontos.

* p1: primeiro ponto
* p2: segundo ponto

Pré-condição: ambos os pontos devem ser válidos
Pós-condição: retorna a distância entre os pontos
*/
double distanciaPontos(Ponto p1, Ponto p2);

/*
Verifica se dois pontos são iguais (mesmas coordenadas).

* p1: primeiro ponto
* p2: segundo ponto

Pré-condição: ambos os pontos devem ser válidos
Pós-condição: retorna true se os pontos são iguais (com tolerância de 1e-9)
*/
bool pontosIguais(Ponto p1, Ponto p2);

/*
Cria uma cópia de um ponto.

* p: ponto a ser copiado

Pré-condição: p deve ser válido
Pós-condição: retorna um novo ponto com as mesmas coordenadas
*/
Ponto copiaPonto(Ponto p);

#endif