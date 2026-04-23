#ifndef CAIXA_H_INCLUDED
#define CAIXA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Pessoa.h"

//estrutura para um no de uma caixa

typedef struct NoCaixa {
    Pessoa *cliente; //ponteiro para Pessoa em UniversoClientes
    int tempoAtendimentoDecorrido; //tempo ja passado
    struct NoCaixa *prox; 
} NoCaixa;

//fila de clientes da caixa

typedef struct {
    NoCaixa *inicio; // Primeiro da fila
    NoCaixa *fim; // Último da fila
    int tamanho; // Número de pessoas na fila
} FilaCaixa;

//estrutura da caixa

typedef struct{
    int id;
    char *operador;
    int operadorID;
    FilaCaixa *fila;
    Pessoa *emAtendimento; //clientes em estado 1 ou 2
    int ativa; //1 = aberta e 2 = fechada
    int clientesAtendidos;
    int produtosVendidos;
    float revenue; //receita total da caixa
} Caixa;

//funções

Caixa* CriarCaixa(int id, const char *operador,int operadorID);

void DestruirCaixa(Caixa *c);

void AdicionarClienteFila(Caixa *c, Pessoa *cliente); //muda estado de 0->1

Pessoa* IniciarAtendimentoProximoCliente(Caixa *c); //muda estado de 1->2

int TamanhoDaFila(Caixa *c);

void IncrementarTempoAtendimento(Caixa *c); //incrementar tempo de atendimento do cliente no estado 2

Pessoa*FinalizarAtendimentoCliente(Caixa *c); //muda estado de 2->3

Pessoa* ObterClienteEmAtendimento(Caixa *c);

#endif