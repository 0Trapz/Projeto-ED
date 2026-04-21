#ifndef PRODUTO_H_INCLUDED
#define PRODUTO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char *nome;
    float preco;
    int tempo_compra; //tempo na loja a escolher o produto
    float tempo_caixa; //tempo na caixa a pagar o produto
} Produto;

typedef struct No {
    void *dados;
    struct No *proximo;
} No;

typedef struct {
    No *inicio;
    No *fim;
    int tamanho;
} Lista;

//Funções
Produto* CriarProduto(int id, char * nome, float preco, int tempo_compra, float tempo_caixa);
void DestruirProduto(Produto *p);
Produto* CopiarProduto(Produto *p);
Produto* ObterProdutoMaisBarato(void *lista);

#endif

##fsdfsdfdsf