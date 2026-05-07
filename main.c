#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Supermercado.h"
#include "Uteis.h"

int Menu()
{
    printf("0 - Sair\n");
    printf("1 - Listar\n");
    printf("2 - Mostrar supermercado\n");
    printf("3 - Mostrar funcionarios\n");
    int OP = LerInteiro("Qual a Opcao ?");
    return OP;
}
void ExecutaAccoesMenu(Supermercado *S)
{
    if (!S) return;

    int OP = Menu();
    switch(OP)
    {
        case 1:
            printf("Funcionalidade de listar clientes ainda por concluir.\n");
            break;
        case 2:
            MostrarSupermercado(S);
            break;
        case 3:
            MostrarFuncionarios(S);
            break;
        case 0: break;
        default:
            printf("Opcao invalida.\n");
            break;
    }

}

int main()
{
    printf("Projeto ED - 25-26!\n");
    srand(time(NULL));
    Supermercado *Lidl = CriarSupermercado("Lidl");
    if (!Lidl) {
        printf("Erro a criar o supermercado.\n");
        return 1;
    }

    if (!InicializarSupermercado(Lidl, "config.txt")) {
        printf("Erro a inicializar o supermercado.\n");
        DestruirSupermercado(Lidl);
        return 1;
    }

    if (!CarregarFuncionarios(Lidl, "funcionarios.txt")) {
        printf("Aviso: nao foi possivel carregar funcionarios.\n");
    }

    Produto produtosTemp[MAX_PRODUTOS_FICHEIRO];
    int totalProdutos = CarregarProdutosDeFicheiro("produtos.txt", produtosTemp, MAX_PRODUTOS_FICHEIRO);
    if (totalProdutos <= 0) {
        printf("Erro a carregar produtos.\n");
        DestruirSupermercado(Lidl);
        return 1;
    }

    Lidl->produtosDisponiveis = (Produto *)malloc(sizeof(Produto) * (size_t)totalProdutos);
    if (!Lidl->produtosDisponiveis) {
        printf("Erro a reservar memoria para produtos.\n");
        DestruirSupermercado(Lidl);
        return 1;
    }
    memcpy(Lidl->produtosDisponiveis, produtosTemp, sizeof(Produto) * (size_t)totalProdutos);
    Lidl->TotalProdutosDisponiveis = totalProdutos;

    Lidl->universoClientes = carregarUniversoClientes("clientes.txt");
    if (Lidl->universoClientes.array == NULL || Lidl->universoClientes.total <= 0) {
        printf("Erro a carregar clientes.\n");
        DestruirSupermercado(Lidl);
        return 1;
    }

    if (!InicializarCaixasSupermercado(Lidl)) {
        printf("Erro a inicializar caixas.\n");
        DestruirSupermercado(Lidl);
        return 1;
    }

    int Terminar = 0;
    while (!Terminar)
    {
        if (TeclaPressionada())
        {
            ExecutaAccoesMenu(Lidl);
        }
        ExecutarSimulacao(Lidl);
        wait_segundos(1);
        Terminar = Supermercado_E_Para_Fechar(Lidl);
    }
    DestruirSupermercado(Lidl);
    return 0;
}
