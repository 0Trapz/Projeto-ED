#ifndef PRODUTO_H_INCLUDED
#define PRODUTO_H_INCLUDED

# define MAX_PRODUTOS_FICHEIRO 10000

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Relogio.h"
#include "Uteis.h"

//------------------------------------------------------------------------------
// Estrutura que representa um produto no supermercado
//------------------------------------------------------------------------------

// Estrutura para armazenar informações sobre um produto
typedef struct {
    int id;
    char nome[128];
    float preco;
    float tempo_compra;
    float tempo_caixa;
} Produto;

//------------------------------------------------------------------------------
// Protótipos de funções
//------------------------------------------------------------------------------

// Carregar produtos de ficheiro
int CarregarProdutosDeFicheiro(const char *ficheiro, Produto *produtos, int maxProdutos);

// Sortear X produtos para cliente e calcular totais
int SortearProdutosParaCliente(const Produto *produtos, int totalProdutos, int quantidade, Produto *produtosCliente, int maxProdutosCliente, float *totalPreco, float *totalTempoCompra, float *totalTempoCaixa);

// Obter produto mais barato
Produto* ObterProdutoMaisBarato(const Produto *produtos, int totalProdutos);

// Obter produto aleatório
Produto* ObterProdutoAleatorio(const Produto *produtos, int totalProdutos);

#endif