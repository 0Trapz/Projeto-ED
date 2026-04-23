#ifndef PRODUTO_H_INCLUDED
#define PRODUTO_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Relogio.h"
#include "Uteis.h"

//------------------------------------------------------------------------------
// Estruturas
//------------------------------------------------------------------------------

// Estrutura que representa um produto no supermercado
typedef struct {
    int id;
    char *nome;
    float preco;
    int tempo_compra; //tempo na loja a escolher o produto
    float tempo_caixa; //tempo na caixa a pagar o produto
} Produto;

// Estrutura de lista ligada para produtos disponíveis no supermercado
typedef struct No {
    void *dados;
    struct No *proximo;
} No;

// Estrutura de lista ligada para produtos disponíveis no supermercado
typedef struct {
    No *inicio;
    No *fim;
    int tamanho;
} Lista;

//------------------------------------------------------------------------------
// Protótipos de funções
//------------------------------------------------------------------------------

// 1. Criar produto
Produto* CriarProduto(int id, char * nome, float preco, int tempo_compra, float tempo_caixa);

// 2. Destruir produto
void DestruirProduto(Produto *p);

// 3. Copiar produto
Produto* CopiarProduto(Produto *p);

// 4. Obter o produto mais barato da lista
int ObterProdutoMaisBarato(Lista *lista);

#endif