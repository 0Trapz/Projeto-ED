#include "Produto.h"

// 1. Carregar produtos de ficheiro
int CarregarProdutosDeFicheiro(const char *ficheiro, Produto *produtos, int maxProdutos) {
    FILE *f = fopen(ficheiro, "r");
    if (!f) return 0;
    char nome[128];
    int id;
    float preco, tempo_compra, tempo_caixa;
    int total = 0;
    while (fscanf(f, "%d %127s %f %f %f", &id, nome, &preco, &tempo_compra, &tempo_caixa) == 5) {
        produtos[total].id = id;
        strncpy(produtos[total].nome, nome, 127);
        produtos[total].nome[127] = '\0';
        produtos[total].preco = preco;
        produtos[total].tempo_compra = tempo_compra;
        produtos[total].tempo_caixa = tempo_caixa;
        total++;
        if (total >= maxProdutos) break;
    }
    fclose(f);
    return total;
}

// 2. Sorteia X produtos aleatórios e calcula totais
void SortearProdutosParaCliente(const Produto *produtos, int totalProdutos, int quantidade, float *totalPreco, float *totalTempoCompra, float *totalTempoCaixa) {
    if (totalProdutos == 0 || quantidade <= 0) {
        if (totalPreco) *totalPreco = 0;
        if (totalTempoCompra) *totalTempoCompra = 0;
        if (totalTempoCaixa) *totalTempoCaixa = 0;
        return;
    }
    float preco = 0, tempoCompra = 0, tempoCaixa = 0;
    for (int i = 0; i < quantidade; i++) {
        Produto *p = ObterProdutoAleatorio(produtos, totalProdutos);
        if (p) {
            preco += p->preco;
            tempoCompra += p->tempo_compra;
            tempoCaixa += p->tempo_caixa;
        }
    }
    if (totalPreco) *totalPreco = preco;
    if (totalTempoCompra) *totalTempoCompra = tempoCompra;
    if (totalTempoCaixa) *totalTempoCaixa = tempoCaixa;
}

// 3. Obter produto mais barato
Produto* ObterProdutoMaisBarato(const Produto *produtos, int totalProdutos) {
    if (totalProdutos == 0) return NULL;
    Produto *mais_barato = (Produto*)&produtos[0];
    for (int i = 1; i < totalProdutos; i++) {
        if (produtos[i].preco < mais_barato->preco) {
            mais_barato = (Produto*)&produtos[i];
        }
    }
    return mais_barato;
}

// 4. Obter produto aleatório
Produto* ObterProdutoAleatorio(const Produto *produtos, int totalProdutos) {
    if (totalProdutos == 0) return NULL;
    int idx = GerarNumeroAleatorio(0, totalProdutos - 1);
    return (Produto*)&produtos[idx];
}