
#include "Supermercado.h"


Supermercado *CriarSupermercado(char *nome)
{
    ptSupermercado s;
    if(nome == NULL)
    return NULL;
    
    s = (Supermercado *)malloc(sizeof(Supermercado));
    if(s == NULL)
    return NULL;

    snprintf(s->nome, 50 + 1, "%s", nome);

    s->relogio = NULL;
    //s->caixas = NULL;

    s->config.maxEspera = 0;
    s->config.nCaixas = 0;
    s->config.maxFila = 0;
    s->config.minFila = 0;
    s->config.tempoAtendimentoProduto = 0;
    s->config.cadenciaEntradaClientes = 0;
    s->config.horaAbertura = 0;
    s->config.horaFecho = 0;

    s->totalClientesAtendidos = 0;
    s->totalProdutosVendidos = 0;
    s->totalProdutosOferecidos = 0;

    return s;

}
int InicializarSupermercado(Supermercado *s, char *config)
{
    FILE *f;
    int i;

    if (s == NULL || config == NULL)
        return 0;

        f = fopen(config, "r");
        if (f == NULL)
        return 0;

        if (fscanf(f, "%d", &s->config.maxEspera) != 1){
            fclose(f);
            return 0;
        }

        if(scanf(f, "%d", &s->config.nCaixas) != 1){
            fclose(f);
            return 0;
        }

        if (scanf(f, "%d", &s->config.maxFila) != 1){
            fclose(f);
            return 0;
        }

        if (scanf(f, "%d", &s->config.minFila) != 1){
            fclose(f);
            return 0;
        }

        if (scanf(f, "%d", &s->config.tempoAtendimentoProduto) != 1){
            fclose(f);
            return 0;
        }

        if (fscanf(f, "%d", &s->config.cadenciaEntradaClientes) !=1){
            fclose(f);
            return 0;
        }
        if (fscanf(f, "%d", &s->config.horaAbertura) != 1){
            fclose(f);
            return 0;
        }

       if (fscanf(f, "%d", &s->config.horaFecho) != 1){
            fclose(f);
            return 0;
        }

        fclose(f);

        s->relogio = CriarRelogio(s->config.horaAbertura, 0, 0);
        if (s->relogio == NULL)
        return 0;

        s->caixas = (ptcaixa) malloc (sizeof(Caixa) * s->config.nCaixas);
        if (s->caixas == NULL)
        return 0;

        for(i = 0; i< s-> config.nCaixas; i++){
            s->caixas[i] = CriarCaixa(i+1);
            if (s->caixas[i] == NULL)
            return 0;
        }
        return 1;


        
}
int ExecutarSimulacao(Supermercado *s)
{
    printf("Estou a trabalhar...\n");
    EntradaPessoaSupermercado(s);

    EstadoPagamentoIrCaixa(s);

    return 1;
}
/*
int IrCaixa(Pessoa *P, Supermercado *s)
{
    //time_t T = GetTempo(S->Rolex);
    //Se (T >=  gfdglkfdglkfdg)
    //    return 1;
    return 0;
}
*/

void EstadoPagamentoIrCaixa(Supermercado *s)
{
    //Para todas as Pessoas P da S->LClientes
    //    Se (IrCaixa(P, S))
        {
            // Escolher Caixa e Retirar essa pessoa de S->Clientes
            // e ir para o Hashing das Caixas
        }
}
void EntradaPessoaSupermercado(Supermercado *s)
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

int Supermercado_E_Para_Fechar(Supermercado *s
{
        //... fazer
    return 0;
}

void DestruirSupermercado(Supermercado *s)
{
    free(s->Rolex);
    free(s);
}
