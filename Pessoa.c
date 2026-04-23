#include "Pessoa.h"
#include "Uteis.h"

//------------------------------------------------------------------------------
// Funções de universo de clientes
//------------------------------------------------------------------------------
UniversoClientes carregarUniversoClientes(const char *ficheiro) {
    UniversoClientes universo = {NULL, 0};
    FILE *f = fopen(ficheiro, "r");
    if (!f) return universo;
    // Conta linhas para alocar array
    int total = 0;
    char linha[256];
    while (LerLinha(linha, sizeof(linha))) total++;
    rewind(f);
    universo.array = (Pessoa*)calloc(total, sizeof(Pessoa));
    universo.total = total;
    int idx = 0;
    while (LerLinha(linha, sizeof(linha)) && idx < total) {
        char id[16], nome[128];
        if (sscanf(linha, "%15s %[^]", id, nome) == 2) {
            strncpy(universo.array[idx].id, id, 15);
            universo.array[idx].id[15] = '\0';
            strncpy(universo.array[idx].nome, nome, 127);
            universo.array[idx].nome[127] = '\0';
            universo.array[idx].numProdutos = 0;
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

    // Sorteia produtos do universo e calcula caracteristicas desta ida do cliente
    float tempoCompraF = 0.0f;
    float tempoCaixaF = 0.0f;
    p->numProdutos = SortearProdutosParaCliente(produtosDisponiveis, totalProdutosDisponiveis, numProdutos, NULL, 0, &p->totalGasto, &tempoCompraF, &tempoCaixaF);
    p->tempoCompra = (int)tempoCompraF;
    p->tempoCaixa = (int)tempoCaixaF;

    // Desconta o produto mais barato
    Produto *maisBarato = ObterProdutoMaisBarato(produtosDisponiveis, totalProdutosDisponiveis);
    if (maisBarato) p->totalGasto -= maisBarato->preco;
    if (p->totalGasto < 0) p->totalGasto = 0;
    p->estado = 0;
    p->countVezesIda = origem->countVezesIda + 1;
    p->totalGastoHistorico = origem->totalGastoHistorico;
    p->totalTempoHistorico = origem->totalTempoHistorico;
    return p;
}

void libertarPessoa(Pessoa *p) {
    if (!p) return;
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
            atual->cliente->estado = 3; 
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
void mostrarClienteAtivo(const Pessoa *p, NodoCliente *ativos) {
    if (!p) return;
    printf("ID: %s\n", p->id);
    printf("Nome: %s\n", p->nome);
    printf("Total gasto: %.2f\n", p->totalGasto);
    printf("Tempo de compra: %d\n", p->tempoCompra);
    printf("Tempo de caixa: %d\n", p->tempoCaixa);
    printf("Estado: %d\n", p->estado);
    printf("Entradas: %d\n", p->countVezesIda);
}

void mostrarClienteHistorico(const Pessoa *p, NodoClienteHistorico *historico) {
    if (!p) return;
    printf("ID: %s\n", p->id);
    printf("Nome: %s\n", p->nome);
    printf("Total gasto: %.2f\n", p->totalGastoHistorico);
    printf("Tempo de compra: %d\n", p->tempoCompra);
    printf("Tempo de caixa: %d\n", p->tempoCaixa);
    printf("Estado: %d\n", p->estado);
    printf("Entradas: %d\n", p->countVezesIda);
}

//------------------------------------------------------------------------------
// Função de registo CSV
//------------------------------------------------------------------------------
void registarAcaoCSV(const char *ficheiro, const char *acao, const Pessoa *cliente) {
    if (!ficheiro || !acao || !cliente) return;
    FILE *f = fopen(ficheiro, "a");
    if (!f) return;
    fprintf(f, "%s;%s;%s;%d;%d;%.2f;%d;%d\n", acao, cliente->id, cliente->nome, cliente->estado, cliente->estado, cliente->totalGasto, cliente->tempoCompra, cliente->tempoCaixa);
    fclose(f);
}