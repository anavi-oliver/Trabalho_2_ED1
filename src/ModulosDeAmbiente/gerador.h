#ifndef GERADOR_H
#define GERADOR_H

/*
*        MÓDULO GERADOR DE IDs ÚNICOS
*
*        Este módulo gerencia a geração de identificadores únicos para
*        formas geométricas. É especialmente importante para:
*        - Segmentos gerados ao transformar círculos em anteparos
*        - Formas clonadas pela bomba de clonagem
*        
*        O gerador mantém controle do próximo ID disponível.
*/

typedef void *Gerador;

/*                    FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO                    */

/*
Cria um novo gerador de IDs.

* idInicial: valor inicial para começar a sequência de IDs

Pré-condição: idInicial >= 0
Pós-condição: retorna um gerador iniciando em idInicial
*/
Gerador criaGerador(int idInicial);

/*
Libera a memória do gerador.

* g: ponteiro para o gerador

Pré-condição: g deve ser válido ou NULL
Pós-condição: memória liberada
*/
void destroiGerador(Gerador g);

/*                    OPERAÇÕES DE GERAÇÃO                    */

/*
Gera um novo ID único e incrementa o contador interno.

* g: ponteiro para o gerador

Pré-condição: g deve ser válido
Pós-condição: retorna um novo ID e incrementa o contador
*/
int geraProximoId(Gerador g);

/*
Retorna o próximo ID que será gerado sem incrementar o contador.

* g: ponteiro para o gerador

Pré-condição: g deve ser válido
Pós-condição: retorna o próximo ID sem modificar o gerador
*/
int consultaProximoId(Gerador g);

/*
Define o próximo ID a ser gerado.
Útil quando se conhece o maior ID existente.

* g: ponteiro para o gerador
* proximoId: próximo ID a ser usado

Pré-condição: g deve ser válido, proximoId >= 0
Pós-condição: próximo ID gerado será >= proximoId
*/
void setProximoId(Gerador g, int proximoId);

/*
Reserva múltiplos IDs de uma vez.
Retorna o primeiro ID da sequência.

* g: ponteiro para o gerador
* quantidade: quantos IDs reservar

Pré-condição: g válido, quantidade > 0
Pós-condição: retorna primeiro ID, avança contador em 'quantidade'
*/
int reservaIds(Gerador g, int quantidade);

#endif