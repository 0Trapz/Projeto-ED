#include "Produto.h"

// Carregar produtos de ficheiro
int CarregarProdutosDeFicheiro(const char *ficheiro, Produto *produtos, int maxProdutos) {
    if (!ficheiro || !produtos || maxProdutos <= 0) return 0;
    FILE *f = fopen(ficheiro, "r");
    if (!f) return 0;
    char linha[512];
    char nome[128];
    int id;
    float preco, tempo_compra, tempo_caixa;
    int total = 0;
    int limite = maxProdutos;
    if (limite > MAX_PRODUTOS_FICHEIRO) limite = MAX_PRODUTOS_FICHEIRO;
   while (total < limite && LerLinhaFicheiro(f, linha, sizeof(linha))) {
        if (sscanf(linha, "%d %127s %f %f %f", &id, nome, &preco, &tempo_compra, &tempo_caixa) == 5) {
            produtos[total].id = id;
            strncpy(produtos[total].nome, nome, 127);
            produtos[total].nome[127] = '\0';
            produtos[total].preco = preco;
            produtos[total].tempo_compra = tempo_compra;
            produtos[total].tempo_caixa = tempo_caixa;
            total++;
        }
    }
    fclose(f);
    return total;
}

// Sorteia X produtos aleatórios para o cliente e calcula totais
int SortearProdutosParaCliente(const Produto *produtos, int totalProdutos, int quantidade, Produto *produtosCliente, int maxProdutosCliente, float *totalPreco, float *totalTempoCompra, float *totalTempoCaixa) {
    if (totalPreco) *totalPreco = 0;
    if (totalTempoCompra) *totalTempoCompra = 0;
    if (totalTempoCaixa) *totalTempoCaixa = 0;
    if (!produtos || totalProdutos <= 0 || quantidade <= 0) return 0;
    int sorteados = quantidade;
    if (produtosCliente && maxProdutosCliente > 0 && sorteados > maxProdutosCliente) {
        sorteados = maxProdutosCliente;
    }
    float preco = 0, tempoCompra = 0, tempoCaixa = 0;
    for (int i = 0; i < sorteados; i++) {
        Produto *p = ObterProdutoAleatorio(produtos, totalProdutos);
        if (!p) continue;
        if (produtosCliente && maxProdutosCliente > 0) {
            produtosCliente[i] = *p;
        }
        preco += p->preco;
        tempoCompra += p->tempo_compra;
        tempoCaixa += p->tempo_caixa;
    }
    if (totalPreco) *totalPreco = preco;
    if (totalTempoCompra) *totalTempoCompra = tempoCompra;
    if (totalTempoCaixa) *totalTempoCaixa = tempoCaixa;
    return sorteados;
}

// Obter produto mais barato
Produto* ObterProdutoMaisBarato(const Produto *produtos, int totalProdutos) {
    if (!produtos || totalProdutos <= 0) return NULL;
    Produto *mais_barato = (Produto*)&produtos[0];
    for (int i = 1; i < totalProdutos; i++) {
        if (produtos[i].preco < mais_barato->preco) {
            mais_barato = (Produto*)&produtos[i];
        }
    }
    return mais_barato;
}

// Obter produto aleatório
Produto* ObterProdutoAleatorio(const Produto *produtos, int totalProdutos) {
    if (!produtos || totalProdutos <= 0) return NULL;
    int idx = Aleatorio(0, totalProdutos - 1);
    return (Produto*)&produtos[idx];
}