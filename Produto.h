#ifndef PRODUTO_H_INCLUDED
#define PRODUTO_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Relogio.h"
#include "Uteis.h"

//------------------------------------------------------------------------------
// Estrutura que representa um produto no supermercado
//------------------------------------------------------------------------------

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

// 1. Carregar produtos de ficheiro
int CarregarProdutosDeFicheiro(const char *ficheiro, Produto *produtos, int maxProdutos);

// 2. Sortear produtos para cliente
void SortearProdutosParaCliente(const Produto *produtos, int totalProdutos, int quantidade, float *totalPreco, float *totalTempoCompra, float *totalTempoCaixa);

// 3. Obter produto mais barato
Produto* ObterProdutoMaisBarato(const Produto *produtos, int totalProdutos);

// 4. Obter produto aleatório
Produto* ObterProdutoAleatorio(const Produto *produtos, int totalProdutos);

#endif