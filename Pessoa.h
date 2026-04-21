// Funções para lista de clientes ativos no supermercado
void inicializarListaClientesAtivos(NodoCliente **lista);
void inserirClienteAtivo(NodoCliente **lista, Pessoa *cliente);
void removerClienteAtivoPorId(NodoCliente **lista, const char *id);
void liberarListaClientesAtivos(NodoCliente **lista);
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
    char id[16];           // Identificador do cliente (ex: P1, P21)
    int numProdutos;       // Número de produtos
    Produto *produtos;     // Array dinâmico de produtos
    float totalGasto;      // Soma dos preços dos produtos
    int tempoCompra;       // Soma dos tempos de compra dos produtos
    int tempoCaixa;        // Soma dos tempos de caixa dos produtos
    int estado;           // Estado do cliente (ex: 0 = compras, 1 = espera, 2 = atendimento, 3 = out, etc.)
    int countVezesIdas;   // Contador de vezes que o cliente foi ao supermercado
    // Outros campos úteis podem ser adicionados (ex: estado, tempo de espera, etc.)
} Pessoa;

// Estrutura para o universo de clientes (todos os clientes lidos do ficheiro)
typedef struct {
    Pessoa* array;   // Array de pessoas
    int total        // Número total de pessoas
} UniversoClientes;

// Estrutura de lista ligada para clientes ativos no supermercado
typedef struct NodoCliente {
    Pessoa* cliente;
    struct NodoCliente* prox;
} NodoCliente;

//------------------------------------------------------------------------------
// Protótipos de funções
//------------------------------------------------------------------------------

// 1. Carregar universo de clientes do ficheiro
UniversoClientes carregarUniversoClientes(const char *ficheiro, Produto *produtosDisponiveis, int totalProdutosDisponiveis);

// 2. Libertar universo de clientes
void libertarUniversoClientes(UniversoClientes *universo);

// 3. Criar pessoa a partir de uma linha do ficheiro
Pessoa* criarPessoaDeLinha(const char *linha, Produto *produtosDisponiveis, int totalProdutosDisponiveis);

// 4. Criar pessoa com produtos aleatórios (opcional, útil para simulação)
Pessoa* criarPessoaAleatoria(const char *id, int numProdutos, Produto *produtosDisponiveis, int totalProdutosDisponiveis);

// 5. Libertar a memória de uma pessoa
void libertarPessoa(Pessoa *p);

// 6. Mostrar os dados de uma pessoa
void mostrarPessoa(const Pessoa *p);

// 7. Calcular totais (gasto, tempo de compra, tempo de caixa)
void calcularTotaisPessoa(Pessoa *p);

// 8. Verificar se a pessoa tem um produto específico (por código)
int pessoaTemProduto(const Pessoa *p, const char *codigoProduto);

// 9. Oferecer o produto mais barato a uma pessoa (remove do total gasto e devolve o ponteiro para o produto oferecido)
Produto* oferecerProdutoMaisBarato(Pessoa *p);

// 10. Adicionar cliente à lista ligada de clientes ativos
void adicionarClienteAtivo(NodoCliente **lista, Pessoa *cliente);

// 11. Remover cliente da lista ligada de clientes ativos (por id)
void removerClienteAtivo(NodoCliente **lista, const char *id);

// 12. Libertar toda a lista ligada de clientes ativos
void libertarListaClientesAtivos(NodoCliente **lista);

#endif // PESSOA_H_INCLUDED