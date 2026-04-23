#include "Pessoa.h"

//------------------------------------------------------------------------------
// Funções de universo de clientes
//------------------------------------------------------------------------------
UniversoClientes carregarUniversoClientes(const char *ficheiro) {
    UniversoClientes universo = {NULL, 0};
    FILE *f = fopen(ficheiro, "r");
    if (!f) return universo;

    int total = 0;
    char linha[256];
    while (fgets(linha, sizeof(linha), f)) total++;
    rewind(f);

    universo.array = (Pessoa*)calloc(total, sizeof(Pessoa));
    universo.total = total;

    int idx = 0;
    while (fgets(linha, sizeof(linha), f) && idx < total) {
        char id[16], nome[128];
        if (sscanf(linha, "%15s %[^]", id, nome) == 2) {
            strncpy(universo.array[idx].id, id, 15);
            universo.array[idx].id[15] = '\0';
            strncpy(universo.array[idx].nome, nome, 127);
            universo.array[idx].nome[127] = '\0';
            universo.array[idx].numProdutos = 0;
            universo.array[idx].produtos = NULL;
            universo.array[idx].totalGasto = 0;
            universo.array[idx].tempoCompra = 0;
            universo.array[idx].tempoCaixa = 0;
            universo.array[idx].estado = 0;
            universo.array[idx].countVezesIda = 0;
            idx++;
        }
    }
    fclose(f);
    universo.total = idx;
    return universo;
}

void libertarUniversoClientes(UniversoClientes *universo) {
    if (!universo || !universo->array) return;
    free(universo->array);
    universo->array = NULL;
    universo->total = 0;
}

//------------------------------------------------------------------------------
// Funções de criação e destruição de clientes
//------------------------------------------------------------------------------
Pessoa* criarClienteAtivoDoUniverso(const UniversoClientes *universo, int idx, Produto *produtosDisponiveis, int totalProdutosDisponiveis, int numProdutos) {
    if (!universo || idx < 0 || idx >= universo->total || !produtosDisponiveis || totalProdutosDisponiveis <= 0 || numProdutos <= 0) return NULL;
    Pessoa *origem = &universo->array[idx];
    Pessoa *p = (Pessoa *)malloc(sizeof(Pessoa));
    if (!p) return NULL;
    strncpy(p->id, origem->id, 15);
    p->id[15] = '\0';
    strncpy(p->nome, origem->nome, 127);
    p->nome[127] = '\0';
    p->numProdutos = numProdutos;
    p->produtos = (Produto *)malloc(numProdutos * sizeof(Produto));
    if (!p->produtos) { free(p); return NULL; }
    for (int i = 0; i < numProdutos; i++) {
        int idxProduto = rand() % totalProdutosDisponiveis;
        p->produtos[i] = produtosDisponiveis[idxProduto];
    }
    p->totalGasto = 0.0f;
    p->tempoCompra = 0;
    p->tempoCaixa = 0;
    p->estado = 0;
    p->countVezesIda = origem->countVezesIda + 1;
    p->totalGastoHistorico = origem->totalGastoHistorico;
    p->totalTempoHistorico = origem->totalTempoHistorico;
    return p;
}

void libertarPessoa(Pessoa *p) {
    if (!p) return;
    if (p->produtos) free(p->produtos);
    free(p);
}

//------------------------------------------------------------------------------
// Funções de listas ligadas (ativos e histórico)
//------------------------------------------------------------------------------
void adicionarClienteAtivo(NodoCliente **lista, Pessoa *cliente) {
    if (!cliente) return;
    NodoCliente *novo = (NodoCliente *)malloc(sizeof(NodoCliente));
    if (!novo) return;
    novo->cliente = cliente;
    novo->prox = *lista;
    *lista = novo;
}

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

Pessoa* procurarClienteAtivo(NodoCliente *ativos, const char *id) {
    while (ativos) {
        if (strcmp(ativos->cliente->id, id) == 0) return ativos->cliente;
        ativos = ativos->prox;
    }
    return NULL;
}

void adicionarClienteHistorico(NodoCliente **historico, Pessoa *cliente) {
    if (!cliente) return;
    NodoCliente *novo = (NodoCliente *)malloc(sizeof(NodoCliente));
    if (!novo) return;
    novo->cliente = cliente;
    novo->prox = *historico;
    *historico = novo;
}

void libertarListaClientesHistorico(NodoCliente **historico) {
    if (!historico) return;
    NodoCliente *atual = *historico;
    while (atual) {
        NodoCliente *temp = atual;
        atual = atual->prox;
        libertarPessoa(temp->cliente);
        free(temp);
    }
    *historico = NULL;
}

void moverClienteParaHistorico(NodoCliente **ativos, NodoCliente **historico, const char *id) {
    if (!ativos || !*ativos || !historico || !id) return;
    NodoCliente *atual = *ativos, *anterior = NULL;
    while (atual) {
        if (strcmp(atual->cliente->id, id) == 0) {
            if (anterior) anterior->prox = atual->prox;
            else *ativos = atual->prox;
            atual->cliente->estado = 3; // Exemplo: 3 = histórico
            adicionarClienteHistorico(historico, atual->cliente);
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
}

Pessoa* procurarClienteHistorico(NodoCliente *historico, const char *id) {
    while (historico) {
        if (strcmp(historico->cliente->id, id) == 0) return historico->cliente;
        historico = historico->prox;
    }
    return NULL;
}

void removerClienteHistorico(NodoCliente **historico, const char *id) {
    if (!historico || !*historico || !id) return;
    NodoCliente *atual = *historico, *anterior = NULL;
    while (atual) {
        if (strcmp(atual->cliente->id, id) == 0) {
            if (anterior) anterior->prox = atual->prox;
            else *historico = atual->prox;
            libertarPessoa(atual->cliente);
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
}

//------------------------------------------------------------------------------
// Funções utilitárias de cliente
//------------------------------------------------------------------------------
void mostrarPessoa(const Pessoa *p) {
    if (!p) return;
    printf("ID: %s\n", p->id);
    printf("Nome: %s\n", p->nome);
    printf("Número de produtos: %d\n", p->numProdutos);
    printf("Total gasto: %.2f\n", p->totalGasto);
    printf("Tempo de compra: %d\n", p->tempoCompra);
    printf("Tempo de caixa: %d\n", p->tempoCaixa);
    printf("Estado: %d\n", p->estado);
    printf("Entradas: %d\n", p->countVezesIda);
}

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

int pessoaTemProduto(const Pessoa *p, const char *codigoProduto) {
    if (!p || !codigoProduto) return 0;
    for (int i = 0; i < p->numProdutos; i++) {
        if (strcmp(p->produtos[i].nome, codigoProduto) == 0) {
            return 1;
        }
    }
    return 0;
}

Produto* oferecerProdutoMaisBarato(Pessoa *p, Lista *produtosDisponiveis) {
    if (!p || !produtosDisponiveis || produtosDisponiveis->tamanho <= 0) return NULL;
    Produto *mais_barato = ObterProdutoMaisBarato(produtosDisponiveis);
    if (!mais_barato) return NULL;

    // Remover apenas UM produto mais barato da lista
    No *anterior = NULL, *atual = produtosDisponiveis->inicio;
    while (atual) {
        Produto *prod = (Produto *)atual->dados;
        if (prod == mais_barato) {
            // Mostrar produto
            printf("Produto oferecido: %s | Preço: %.2f\n", prod->nome, prod->preco);
            // Atualizar total gasto da pessoa
            p->totalGasto -= prod->preco;
            if (p->totalGasto < 0) p->totalGasto = 0;
            // Remover nó
            if (anterior) anterior->proximo = atual->proximo;
            else produtosDisponiveis->inicio = atual->proximo;
            if (produtosDisponiveis->fim == atual) produtosDisponiveis->fim = anterior;
            produtosDisponiveis->tamanho--;
            Produto *copia = CopiarProduto(prod);
            DestruirProduto(prod);
            free(atual);
            printf("Novo total: %.2f\n", p->totalGasto);
            return copia;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    return NULL;
}

//------------------------------------------------------------------------------
// Função de registo CSV
//------------------------------------------------------------------------------
void registarAcaoCSV(const char *ficheiro, const char *acao, const Pessoa *cliente) {
    if (!ficheiro || !acao || !cliente) return;
    FILE *f = fopen(ficheiro, "a");
    if (!f) return;
    fprintf(f, "%s;%s;%s;%d;%d;%.2f;%d;%d\n", acao, cliente->id, cliente->nome, cliente->numProdutos, cliente->estado, cliente->totalGasto, cliente->tempoCompra, cliente->tempoCaixa);
    fclose(f);
}