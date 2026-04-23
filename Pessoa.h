
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
    int numProdutos;           // Número de produtos
    Produto *produtos;         // Array dinâmico de produtos
    float totalGasto;          // Soma dos preços dos produtos
    int tempoCompra;           // Soma dos tempos de compra dos produtos
    int tempoCaixa;            // Soma dos tempos de caixa dos produtos
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

//------------------------------------------------------------------------------
// Funções de universo de clientes
//------------------------------------------------------------------------------
UniversoClientes carregarUniversoClientes(const char *ficheiro);
void libertarUniversoClientes(UniversoClientes *universo);

//------------------------------------------------------------------------------
// Funções de criação e destruição de clientes
//------------------------------------------------------------------------------
Pessoa* criarClienteAtivoDoUniverso(const UniversoClientes *universo, int idx, Produto *produtosDisponiveis, int totalProdutosDisponiveis, int numProdutos);
void libertarPessoa(Pessoa *p);

//------------------------------------------------------------------------------
// Funções de listas ligadas (ativos e histórico)
//------------------------------------------------------------------------------
void adicionarClienteAtivo(NodoCliente **lista, Pessoa *cliente);
void removerClienteAtivo(NodoCliente **lista, const char *id);
void libertarListaClientesAtivos(NodoCliente **lista);
Pessoa* procurarClienteAtivo(NodoCliente *ativos, const char *id);
void adicionarClienteHistorico(NodoCliente **historico, Pessoa *cliente);
void libertarListaClientesHistorico(NodoCliente **historico);
void moverClienteParaHistorico(NodoCliente **ativos, NodoCliente **historico, const char *id);
Pessoa* procurarClienteHistorico(NodoCliente *historico, const char *id);
void removerClienteHistorico(NodoCliente **historico, const char *id);

//------------------------------------------------------------------------------
// Funções utilitárias de cliente
//------------------------------------------------------------------------------
void mostrarPessoa(const Pessoa *p);
void calcularTotaisPessoa(Pessoa *p);
int pessoaTemProduto(const Pessoa *p, const char *codigoProduto);
Produto* oferecerProdutoMaisBarato(Pessoa *p, Lista *produtosDisponiveis);

//------------------------------------------------------------------------------
// Função de registo CSV
//------------------------------------------------------------------------------
void registarAcaoCSV(const char *ficheiro, const char *acao, const Pessoa *cliente);

#endif // PESSOA_H_INCLUDED