#include "Pessoa.h"

#include "Pessoa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. Carregar universo de clientes do ficheiro (só esqueleto, implementar conforme formato do ficheiro)
UniversoClientes carregarUniversoClientes(const char *ficheiro, Produto *produtosDisponiveis, int totalProdutosDisponiveis) {
    UniversoClientes universo = {NULL, 0};
    // TODO: Implementar leitura do ficheiro e preenchimento do array
    return universo;
}

// 2. Libertar universo de clientes
void libertarUniversoClientes(UniversoClientes *universo) {
    if (!universo || !universo->array) return;
    for (int i = 0; i < universo->total; i++) {
        libertarPessoa(&universo->array[i]);
    }
    free(universo->array);
    universo->array = NULL;
    universo->total = 0;
}

// 3. Criar pessoa a partir de uma linha do ficheiro
Pessoa* criarPessoaDeLinha(const char *linha, Produto *produtosDisponiveis, int totalProdutosDisponiveis) {
    if (linha == NULL || produtosDisponiveis == NULL || totalProdutosDisponiveis <= 0) {
        printf("Linha ou produtos disponíveis inválidos\n");
        return NULL;
    }
    char id[16];
    int numProdutos;
    if (sscanf(linha, "%15s : %d", id, &numProdutos) != 2) {
        printf("Formato da linha inválido: %s\n", linha);
        return NULL;
    }
    return criarPessoaAleatoria(id, numProdutos, produtosDisponiveis, totalProdutosDisponiveis);
}

// 4. Criar pessoa com produtos aleatórios (opcional, útil para simulação)
Pessoa* criarPessoaAleatoria(const char *id, int numProdutos, Produto *produtosDisponiveis, int totalProdutosDisponiveis) {
    if (id == NULL || numProdutos <= 0 || produtosDisponiveis == NULL || totalProdutosDisponiveis <= 0) {
        printf("Parâmetros inválidos para criar pessoa aleatória\n");
        return NULL;
    }
    Pessoa *p = (Pessoa *)malloc(sizeof(Pessoa));
    if (!p) {
        printf("Erro ao alocar memória para pessoa\n");
        return NULL;
    }
    strncpy(p->id, id, 15);
    p->id[15] = '\0';
    p->numProdutos = numProdutos;
    p->produtos = (Produto *)malloc(numProdutos * sizeof(Produto));
    if (!p->produtos) {
        printf("Erro ao alocar memória para produtos da pessoa\n");
        free(p);
        return NULL;
    }
    for (int i = 0; i < numProdutos; i++) {
        int idxProduto = rand() % totalProdutosDisponiveis;
        p->produtos[i] = produtosDisponiveis[idxProduto];
    }
    calcularTotaisPessoa(p);
    return p;
}

// 5. Libertar a memória de uma pessoa
void libertarPessoa(Pessoa *p) {
    if (!p) return;
    if (p->produtos) free(p->produtos);
    free(p);
}

// 6. Mostrar os dados de uma pessoa
void mostrarPessoa(const Pessoa *p) {
    if (!p) return;
    printf("ID: %s\n", p->id);
    printf("Número de produtos: %d\n", p->numProdutos);
    printf("Total gasto: %.2f\n", p->totalGasto);
    printf("Tempo de compra: %d\n", p->tempoCompra);
    printf("Tempo de caixa: %d\n", p->tempoCaixa);
}

// 7. Calcular totais (gasto, tempo de compra, tempo de caixa)
void calcularTotaisPessoa(Pessoa *p) {
    if (!p) return;
    p->totalGasto = 0.0f;
    p->tempoCompra = 0;
    p->tempoCaixa = 0;
    for (int i = 0; i < p->numProdutos; i++) {
        p->totalGasto += p->produtos[i].preco;
        p->tempoCompra += p->produtos[i].tempo_compra;
        p->tempoCaixa += p->produtos[i].tempo_caixa;
    }
}

// 8. Verificar se a pessoa tem um produto específico (por código)
int pessoaTemProduto(const Pessoa *p, const char *codigoProduto) {
    if (!p || !codigoProduto) return 0;
    for (int i = 0; i < p->numProdutos; i++) {
        if (strcmp(p->produtos[i].id, codigoProduto) == 0) return 1;
    }
    return 0;
}

// 9. Oferecer o produto mais barato a uma pessoa
Produto* oferecerProdutoMaisBarato(Pessoa *p) {
    if (!p || p->numProdutos == 0 || !p->produtos) return NULL;
    int idxMaisBarato = 0;
    for (int i = 1; i < p->numProdutos; i++) {
        if (p->produtos[i].preco < p->produtos[idxMaisBarato].preco) idxMaisBarato = i;
    }
    Produto* oferecido = (Produto*)malloc(sizeof(Produto));
    if (!oferecido) return NULL;
    *oferecido = p->produtos[idxMaisBarato];
    for (int i = idxMaisBarato; i < p->numProdutos - 1; i++) {
        p->produtos[i] = p->produtos[i + 1];
    }
    p->numProdutos--;
    p->produtos = realloc(p->produtos, p->numProdutos * sizeof(Produto));
    calcularTotaisPessoa(p);
    return oferecido;
}

// 10. Adicionar cliente à lista ligada de clientes ativos
void adicionarClienteAtivo(NodoCliente **lista, Pessoa *cliente) {
    if (!cliente) return;
    NodoCliente *novo = (NodoCliente *)malloc(sizeof(NodoCliente));
    if (!novo) return;
    novo->cliente = cliente;
    novo->prox = *lista;
    *lista = novo;
}

// 11. Remover cliente da lista ligada de clientes ativos (por id)
void removerClienteAtivo(NodoCliente **lista, const char *id) {
    if (!lista || !*lista || !id) return;
    NodoCliente *atual = *lista, *anterior = NULL;
    while (atual) {
        if (strcmp(atual->cliente->id, id) == 0) {
            if (anterior) anterior->prox = atual->prox;
            else *lista = atual->prox;
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
}

// 12. Libertar toda a lista ligada de clientes ativos
void libertarListaClientesAtivos(NodoCliente **lista) {
    if (!lista) return;
    NodoCliente *atual = *lista;
    while (atual) {
        NodoCliente *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    *lista = NULL;
}