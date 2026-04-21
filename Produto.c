#include "Produto.h"

Produto* CriarProduto(int id, char *nome, float preco, int tempo_compra, float tempo_caixa) {
    Produto *p = (Produto *)malloc(sizeof(Produto));
    if (!p) {
        printf("Erro: Falha ao alocar memória para Produto\n");
        return NULL;
    }

    p->id = id;

    //copiar nome
    p->nome = (char*)malloc(strlen(nome) + 1);
    if (!p->nome) {
        printf("ERRO: Falha ao alocar memória para nome do produto\n");
        free(p);
        return NULL;
    }
    strcpy(p->nome, nome);

    p->preco = preco;
    p->tempo_compra = tempo_compra;
    p->tempo_caixa = tempo_caixa;

    return p;
}

void DestruirProduto(Produto * p) {
    if (p) {
        if (p->nome) {
            free(p->nome);
        }
        free(p);
    }
}

Produto* CopiarProduto(Produto * p) {
    if (!p) return NULL;
    return CriarProduto(p->id, p->nome, p->preco, p->tempo_compra, p->tempo_caixa);
}

Produto* ObterProdutoMaisBarato(Lista *lista) {
    if (!lista || lista->tamanho == 0){
        return NULL;
    }

    Produto *mais_barato = NULL;
    float menor_preco = 999999.0;

    No *atual = lista->inicio;
    while (atual) {
        Produto *prod = (Produto *)atual->dados;
        if (prod && prod->preco < menor_preco) {
            menor_preco = prod->preco;
            mais_barato = prod;
        }
        atual = atual->proximo;
    }
    return mais_barato;
}
