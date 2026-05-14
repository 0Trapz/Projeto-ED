#ifndef CAIXA_H_INCLUDED
#define CAIXA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Pessoa.h"

//------------------------------------------------------------------------------
// Estruturas
//------------------------------------------------------------------------------

// Estrutura para um no de uma caixa
typedef struct NoCaixa {
    Pessoa *cliente; //ponteiro para Pessoa em UniversoClientes
    int tempoAtendimentoDecorrido; //tempo ja passado
    struct NoCaixa *prox; 
} NoCaixa;

// Fila de clientes da caixa
typedef struct {
    NoCaixa *inicio; // Primeiro da fila
    NoCaixa *fim; // Último da fila
    int tamanho; // Número de pessoas na fila
} FilaCaixa;

//Estrutura da caixa
typedef struct{
    int id;
    char *operador;
    int operadorID;
    FilaCaixa *fila;
    Pessoa *emAtendimento; //clientes em estado 1 ou 2
    int tempoAtendimentoDecorrido;
    int ativa; //1 = aberta e 0 = fechada
    int clientesAtendidos;
    int produtosVendidos;
    float revenue; //receita total da caixa
    char motivoFecho[128]; //motivo do fecho da caixa
} Caixa;

//------------------------------------------------------------------------------
// Protótipos de funções
// ------------------------------------------------------------------------------

// Funçao de criação de caixa
Caixa* CriarCaixa(int id, const char *operador,int operadorID);

// Função de destruição de caixa (liberta fila, mas não clientes)
void DestruirCaixa(Caixa *c);

// Funções de atendimento
void AdicionarClienteFila(Caixa *c, Pessoa *cliente); //muda estado de 0->1
Pessoa* RemoverClienteFila(Caixa *c);
Pessoa* ProcurarClienteFila(Caixa *c, const char *id, int *posicao );
Pessoa* RemoverClienteFilaPorID(Caixa *c, const char *id);
Caixa* CaixaComMenorFila(Caixa **caixas, int totalCaixas);
Caixa* AbrirProximaCaixa(Caixa **caixas, int totalCaixas);
int ProcessarCaixa(Caixa *c, Pessoa **clienteFinalizado);

// Retorna cliente que passou para atendimento (estado 2) ou NULL se não houver
Pessoa* IniciarAtendimentoProximoCliente(Caixa *c); //muda estado de 1->2

// Retorna cliente que finalizou atendimento (estado 3) ou NULL se não houver
int TamanhoDaFila(Caixa *c);

// Incrementa tempo de atendimento do cliente em estado 2 (em atendimento)
void IncrementarTempoAtendimento(Caixa *c); //incrementar tempo de atendimento do cliente no estado 2

// Retorna cliente que finalizou atendimento (estado 3) ou NULL se não houver
Pessoa*FinalizarAtendimentoCliente(Caixa *c); //muda estado de 2->3

// Retorna cliente que está em atendimento (estado 2) ou NULL se não houver
Pessoa* ObterClienteEmAtendimento(Caixa *c);

#endif