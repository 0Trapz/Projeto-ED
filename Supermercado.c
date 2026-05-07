#include "Supermercado.h"

static void AplicarConfig(ptCONFIGURACAO config, char *chave, int valor)
{
    if (strcmp(chave, "Max_Espera") == 0) {
        config->maxEspera = valor;
    } else if (strcmp(chave, "nCaixas") == 0) {
        config->nCaixas = valor;
    } else if (strcmp(chave, "maxFila") == 0) {
        config->maxFila = valor;
    } else if (strcmp(chave, "minFila") == 0) {
        config->minFila = valor;
    } else if (strcmp(chave, "tempoAtendimentoProduto") == 0) {
        config->tempoAtendimentoProduto = valor;
    } else if (strcmp(chave, "cadenciaEntradaClientes") == 0) {
        config->cadenciaEntradaClientes = valor;
    } else if (strcmp(chave, "horaAbertura") == 0) {
        config->horaAbertura = valor;
    } else if (strcmp(chave, "horaFecho") == 0) {
        config->horaFecho = valor;
    }
}

Supermercado *CriarSupermercado(char *nome)
{
    ptSupermercado s;
    int i; 

    if(nome == NULL)
    return NULL;
    
    s = (ptSupermercado )malloc(sizeof(Supermercado));
    if(s == NULL)
    return NULL;

    snprintf(s->nome, MAX_NOME_SUPERMERCADO + 1, "%s", nome);
    memset(&s->config, 0, sizeof(CONFIGURACAO));
   

    s->relogio = NULL;  
    s->clientesEmCompras = NULL;
    s->produtosDisponiveis = NULL;
    s->TotalProdutosDisponiveis = 0;

    s->universoClientes.array = NULL;
    s->universoClientes.total = 0;
    s->proximoCliente = 0;

    s->caixas = NULL;

    s->totalFuncionarios = 0;
    for(i = 0; i < MAX_FUNCIONARIOS; i++)
    {
        s->funcionarioEmUso[i] = 0;
        s->funcionarios[i][0] = '\0';
    }


    s->totalClientesAtendidos = 0;
    s->totalProdutosVendidos = 0;
    s->totalProdutosOferecidos = 0;

    return s;

}
int InicializarSupermercado(ptSupermercado s, char *config)
{
    FILE *f;
    char chave[64];
    int valor;

    if (s == NULL || config == NULL)
        return 0;

        f = fopen(config, "r");
        if (f == NULL)
        return 0;

        while (fscanf(f, "%63s %d", chave, &valor) == 2){
            aplicarConfig(&s->config, chave, valor);
        }
        
        
        fclose(f);

        s->relogio = CriarRelogio(s->config.horaAbertura, 0, 0);
        if (s->relogio == NULL)
        return 0;

}

int CarregarFuncionarios(ptSupermercado s, char *nomeFicheiroFuncionarios)
{
    FILE *f;
    char nome[MAX_NOME_FUNCIONARIO + 1];
    int id;

    if (s == NULL || nomeFicheiroFuncionarios == NULL)
        return 0;

    f = fopen(nomeFicheiroFuncionarios, "r");
    if (f == NULL)
        return 0;

        s->totalFuncionarios = 0;

    while (s->totalFuncionarios < MAX_FUNCIONARIOS && fscanf(f, "%d %80[^\n]", &id, nome) == 2) {
        s->idFuncionario[s->totalFuncionarios] = id;
        snprintf(s->funcionarios[s->totalFuncionarios], MAX_NOME_FUNCIONARIO + 1, "%s", nome);
        s->funcionarioEmUso[s->totalFuncionarios] = 0;
        s->totalFuncionarios++;
    }
    fclose(f);
    return 1;
}

int ObterFuncionarioLivre(ptSupermercado s)
{
   int i;
   if(s==NULL) return-1;
   for (i=0;i < s->totalFuncionarios;i++)
    if(s->funcionarioEmUso[i] == 0)
     return i; 
   return -1;
}

int AtribuirFuncionarioLivre(ptSupermercado s)
{
    int indice = ObterFuncionarioLivre(s);
    if (indice != -1) {
        s->funcionarioEmUso[indice] = 1; // Marcar como ocupado
        return indice;
    }
    
}
void libertarFuncionario(ptSupermercado s, int indice)
{
    if (s== NULL) return;
    if (indice <0 || indice >= s->totalFuncionarios) return;
    s->funcionarioEmUso[indice] = 0; // Marcar como livre
}

int FuncionarioEmUso(ptSupermercado s, int idFuncionario)
{
    if (s == NULL) return 0;
    if (idFuncionario < 0 || idFuncionario >= s->totalFuncionarios) return 0;
    return s->funcionarioEmUso[idFuncionario];
}

void MostrarFuncionarios(ptSupermercado s)
{
    int i;
    if (s == NULL) return;

    for (i=0; i <s->totalFuncionarios; i++)
    {
        printf("ID: %d, Nome: %s, Em Uso: %s\n", i, s->funcionarios[i], s->funcionarioEmUso[i] ? "Sim" : "Não");
    }
}

int MostrarSupermercado(ptSupermercado s)
{
    if (s == NULL) return 0;

    printf("Supermercado: %s\n", s->nome);
    printf("Configurações:\n");
    printf("  Max Espera: %d\n", s->config.maxEspera);
    printf("  Número de Caixas: %d\n", s->config.nCaixas);
    printf("  Max Fila: %d\n", s->config.maxFila);
    printf("  Min Fila: %d\n", s->config.minFila);
    printf("  Tempo Atendimento Produto: %d\n", s->config.tempoAtendimentoProduto);
    printf("  Cadência Entrada Clientes: %d\n", s->config.cadenciaEntradaClientes);
    printf("  Hora Abertura: %d\n", s->config.horaAbertura);
    printf("  Hora Fecho: %d\n", s->config.horaFecho);

    return 1;
}

void EntradaPessoaSupermercado(ptSupermercado s){
    int x;
    if (s == NULL) return;

    x = Aleatorio(0, 100);
    if (x<s->config.cadenciaEntradaClientes)
    {

        printf("Um novo cliente entrou no supermercado!\n");
    }
}


int ExecutarSimulacao(ptSupermercado s)
{
    if (s== NULL || s->relogio== NULL ) return 0;
    AvancarRelogio(s->relogio, 1); // Avança o relógio em 1 segundo
    EntradaPessoaSupermercado(s);

    return 1;
}

int Supermercado_E_Para_Fechar(Supermercado *s)
{
    if (s == NULL || s->relogio == NULL) return 1;

    int horaAtual = s->relogio->horas;
    int minutoAtual = s->relogio->minutos;

    if (horaAtual > s->config.horaFecho || (horaAtual == s->config.horaFecho && minutoAtual > 0)) {
        return 1; // Supermercado deve fechar
    }
    return 0; // Supermercado ainda pode permanecer aberto
}

void DestruirSupermercado(ptSupermercado s)
{

    int i;
    if (s == NULL) return;
    if (s->relogio != NULL)
    DestruirRelogio(s->relogio);
    
    if (s->caixas != NULL ){
        for (i=0; i < s->config.nCaixas; i++){
            DestruirCaixa(&s->caixas[i]);
        }free(s->caixas);
    }
libertarListaClientesAtivos(&s->clientesEmCompras);
libertarUniversoClientes(&s->universoClientes);

free(s);

}