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
