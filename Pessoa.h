#ifndef PESSOA_H_INCLUDED
#define PESSOA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Produto.h"

//------------------------------------------------------------------------------
// Estruturas
//------------------------------------------------------------------------------

// Estrutura que representa um cliente/pessoa no supermercado
typedef struct {
    char id[16];               // Identificador do cliente (ex: P1, P21)
    char nome[128];            // Nome do cliente
    int numProdutos;           // Numero de produtos sorteados para a ida atual
    float totalGasto;          // Total gasto na ida atual
    int tempoCompra;           // Tempo de compra na ida atual
    int tempoCaixa;            // Tempo de caixa na ida atual
    int estado;                // Estado do cliente (ex: 0 = compras, 1 = espera, 2 = atendimento, 3 = out, etc.)
    int countVezesIda;         // Contador de vezes que o cliente foi ao supermercado
    int totalGastoHistorico;   // Total gasto acumulado em todas as idas ao supermercado
    int totalTempoHistorico;   // Total de tempo gasto acumulado em todas as idas ao supermercado
    // Outros campos úteis podem ser adicionados (ex: estado, tempo de espera, etc.)
} Pessoa;

// Estrutura para o universo de clientes (todos os clientes lidos do ficheiro)
typedef struct {
    Pessoa* array;    // Array de pessoas
    int total;        // Número total de pessoas
} UniversoClientes;

// Estrutura de lista ligada para clientes ativos no supermercado
typedef struct NodoCliente {
    Pessoa* cliente;             // Ponteiro para a pessoa/cliente
    struct NodoCliente* prox;    // Ponteiro para o próximo nodo na lista
} NodoCliente;

// Estrutura de lista ligada para clientes históricos (que já saíram do supermercado)
typedef struct NodoClienteHistorico {
    Pessoa* cliente;                      // Ponteiro para a pessoa/cliente
    struct NodoClienteHistorico* prox;    // Ponteiro para o próximo nodo na lista
} NodoClienteHistorico;

// ------------------------------------------------------------------------------
// Protótipos de funções
// ------------------------------------------------------------------------------

// Funções de universo de clientes
UniversoClientes carregarUniversoClientes(const char *ficheiro);
void libertarUniversoClientes(UniversoClientes *universo);

// Funções de criação e destruição de clientes
// Agora recebe produtos disponíveis e total como parâmetros
Pessoa* criarClienteAtivoDoUniverso(const UniversoClientes *universo, int idx, Produto *produtosDisponiveis, int totalProdutosDisponiveis, int numProdutos);
void libertarPessoa(Pessoa *p);

// Funções de listas ligadas (ativos e histórico)
void adicionarClienteAtivo(NodoCliente **lista, Pessoa *cliente);
void removerClienteAtivo(NodoCliente **lista, const char *id);
void libertarListaClientesAtivos(NodoCliente **lista);
Pessoa* procurarClienteAtivo(NodoCliente *ativos, const char *id);
void adicionarClienteHistorico(NodoCliente **historico, Pessoa *cliente);
void removerClienteHistorico(NodoCliente **historico, const char *id);
void libertarListaClientesHistorico(NodoCliente **historico);
Pessoa* procurarClienteHistorico(NodoCliente *historico, const char *id);
void moverClienteParaHistorico(NodoCliente **ativos, NodoCliente **historico, const char *id);

// Funções utilitárias de cliente
void mostrarClienteAtivo(const Pessoa *p, NodoCliente *ativos); 
void mostrarClienteHistorico(const Pessoa *p, NodoClienteHistorico *historico);
// Nota: Produtos não são guardados por pessoa; operações são feitas na lista global

// Função de registo CSV
void registarAcaoCSV(const char *ficheiro, const char *acao, const Pessoa *cliente);

#endif // PESSOA_H_INCLUDED