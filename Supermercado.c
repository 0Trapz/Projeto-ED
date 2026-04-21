
#include "Supermercado.h"

extern int Aleatorio(int min, int max);

//---------------------------------------------

Supermercado *CriarSupermercado(char *nome)
{
    Supermercado *S = (Supermercado *)malloc(sizeof(Supermercado));
    //... fazer
    S->Rolex = CriarRelogio(10);
    return S;
}
int InicializarSupermercado(Supermercado *S, char *config)
{
    //... fazer
    // S->HoraInicio = 8;
    S->CadenciaEntradaClientes = 30;
    return 1;
}
int ExecutarSimulacao(Supermercado *S)
{
    printf("Estou a trabalhar...\n");
    EntradaPessoaSupermercado(S);

    EstadoPagamentoIrCaixa(S);

    return 1;
}
/*
int IrCaixa(Pessoa *P, Supermercado *S)
{
    //time_t T = GetTempo(S->Rolex);
    //Se (T >=  gfdglkfdglkfdg)
    //    return 1;
    return 0;
}
*/

void EstadoPagamentoIrCaixa(Supermercado *S)
{
    //Para todas as Pessoas P da S->LClientes
    //    Se (IrCaixa(P, S))
        {
            // Escolher Caixa e Retirar essa pessoa de S->Clientes
            // e ir para o Hashing das Caixas
        }
}
void EntradaPessoaSupermercado(Supermercado *S)
{
    int X = Aleatorio(0, 100);
    //printf("X = %d\n", X);
    if (X < S->CadenciaEntradaClientes)
    {
        //Pessoa *P = CriarPessoa();
        //AddLista(S->LCliente, P);
        printf("Mais um Cliente a Entrar!\n");
    }
    //---------------------
}

int Supermercado_E_Para_Fechar(Supermercado *S)
{
        //... fazer
    return 0;
}

void DestruirSupermercado(Supermercado *S)
{
    free(S->Rolex);
    free(S);
}
