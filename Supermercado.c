#include "Supermercado.h"

void AplicarConfig(ptCONFIGURACAO config, char *chave, int valor)
{
    if (strcmp(chave, "MAX_ESPERA") == 0) {
        config->maxEspera = valor;
    } else if (strcmp(chave, "N_CAIXAS") == 0) {
        config->nCaixas = valor;
    } else if (strcmp(chave, "MAX_FILA") == 0) {
        config->maxFila = valor;
    } else if (strcmp(chave, "MIN_FILA") == 0) {
        config->minFila = valor;
    } else if (strcmp(chave, "TEMPO_ATENDIMENTO_PRODUTO") == 0) {
        config->tempoAtendimentoProduto = valor;
    } else if (strcmp(chave, "CADENCIA_ENTRADA_CLIENTES") == 0) {
        config->cadenciaEntradaClientes = valor;
    } else if (strcmp(chave, "HORA_ABERTURA") == 0 || strcmp(chave, "horaAbertura") == 0) {
        config->horaAbertura = valor;
    } else if (strcmp(chave, "HORA_FECHO") == 0 || strcmp(chave, "horaFecho") == 0) {
        config->horaFecho = valor;
    } else if (strcmp(chave, "MAX_PRECO") == 0) {
        config->maxPreco = valor;
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
int InicializarSupermercado(ptSupermercado s, char *config){
    FILE *f;
    char chave[64];
    int valor;

    if (s == NULL || config == NULL)
        return 0;

    f = fopen(config, "r");
        if (f == NULL)
        return 0;

        while (fscanf(f, "%63s %d", chave, &valor) == 2){
            AplicarConfig(&s->config, chave, valor);
        }
        
        
        fclose(f);

    s->relogio = CriarRelogio(s->config.horaAbertura, 0, 0);
    if (s->relogio == NULL) {
        return 0;
    }
        return 1;

}

int CarregarFuncionarios(ptSupermercado s, char *nomeFicheiroFuncionarios){
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

static void AdicionarClienteAoFim(NodoCliente **lista, Pessoa *cliente)
{
    NodoCliente *novo;
    NodoCliente *atual;

    if (!lista || !cliente) return;

    novo = (NodoCliente *)malloc(sizeof(NodoCliente));
    if (!novo) return;

    novo->cliente = cliente;
    novo->prox = NULL;

    if (*lista == NULL) {
        *lista = novo;
        return;
    }

    atual = *lista;
    while (atual->prox != NULL) {
        atual = atual->prox;
    }
    atual->prox = novo;
}

static Pessoa *RemoverPrimeiroCliente(NodoCliente **lista)
{
    NodoCliente *primeiro;
    Pessoa *cliente;

    if (!lista || !*lista) return NULL;

    primeiro = *lista;
    *lista = primeiro->prox;
    cliente = primeiro->cliente;
    free(primeiro);
    return cliente;
}

int InicializarCaixasSupermercado(ptSupermercado s)
{
    int i;

    if (!s || s->config.nCaixas <= 0) return 0;

    s->caixas = (Caixa **)calloc((size_t)s->config.nCaixas, sizeof(Caixa *));
    if (!s->caixas) return 0;

    for (i = 0; i < s->config.nCaixas; i++) {
        const char *operador = "Sem operador";
        int operadorID = -1;

        if (i < s->totalFuncionarios) {
            operador = s->funcionarios[i];
            operadorID = s->idFuncionario[i];
        }

        s->caixas[i] = CriarCaixa(i + 1, operador, operadorID);
        if (!s->caixas[i]) return 0;
        s->caixas[i]->ativa = (i == 0) ? 1 : 0;
    }

    return 1;
}

static Caixa *ObterCaixaComMenorFila(ptSupermercado s)
{
    Caixa *melhor = NULL;
    int i;

    if (!s || !s->caixas) return NULL;

    for (i = 0; i < s->config.nCaixas; i++) {
        Caixa *atual = s->caixas[i];
        if (!atual) continue;
        if (!melhor || (atual->ativa && TamanhoDaFila(atual) < TamanhoDaFila(melhor))) {
            melhor = atual;
        }
    }

    return melhor;
}

static Caixa *AtivarProximaCaixaSePreciso(ptSupermercado s)
{
    int i;

    if (!s || !s->caixas) return NULL;

    for (i = 0; i < s->config.nCaixas; i++) {
        if (s->caixas[i] && s->caixas[i]->ativa == 0) {
            s->caixas[i]->ativa = 1;
            return s->caixas[i];
        }
    }

    return NULL;
}

static void AdicionarClienteAoSistema(ptSupermercado s, Pessoa *cliente)
{
    if (!s || !cliente) return;
    cliente->estado = 0;
    AdicionarClienteAoFim(&s->clientesEmCompras, cliente);
}

static void DistribuirClientesParaCaixas(ptSupermercado s)
{
    Pessoa *cliente;
    Caixa *caixa;

    if (!s) return;

    while (s->clientesEmCompras) {
        caixa = ObterCaixaComMenorFila(s);
        if (!caixa) return;

        if (caixa->ativa == 0) {
            caixa = AtivarProximaCaixaSePreciso(s);
            if (!caixa) return;
        }

        if (TamanhoDaFila(caixa) >= s->config.maxFila) {
            caixa = AtivarProximaCaixaSePreciso(s);
            if (!caixa) return;
        }

        cliente = RemoverPrimeiroCliente(&s->clientesEmCompras);
        if (!cliente) return;
        AdicionarClienteFila(caixa, cliente);
    }
}

static void ProcessarCaixa(ptSupermercado s, Caixa *caixa)
{
    Pessoa *clienteFinalizado;

    if (!s || !caixa || !caixa->ativa) return;

    if (!ObterClienteEmAtendimento(caixa)) {
        IniciarAtendimentoProximoCliente(caixa);
    }

    if (ObterClienteEmAtendimento(caixa)) {
        IncrementarTempoAtendimento(caixa);
        if (caixa->emAtendimento && caixa->tempoAtendimentoDecorrido >= (int)caixa->emAtendimento->tempoCaixa) {
            clienteFinalizado = FinalizarAtendimentoCliente(caixa);
            if (clienteFinalizado) {
                s->totalClientesAtendidos++;
                s->totalProdutosVendidos += clienteFinalizado->numProdutos;
                s->totalProdutosOferecidos += 1;
                s->custoTotalOfertas += clienteFinalizado->totalGasto;
                clienteFinalizado->totalGastoHistorico += clienteFinalizado->totalGasto;
                clienteFinalizado->totalTempoHistorico += clienteFinalizado->tempoCompra + clienteFinalizado->tempoCaixa;
                adicionarClienteHistorico(&s->clientesHistorico, clienteFinalizado);
            }
        }
    }
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
    return -1; // Nenhum funcionário livre encontrado
    
}
void LibertarFuncionario(ptSupermercado s, int indice)
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
        printf("ID: %d, Nome: %s, Em Uso: %s\n", s->idFuncionario[i], s->funcionarios[i], s->funcionarioEmUso[i] ? "Sim" : "Não");
    }
}

int MostrarSupermercado(ptSupermercado s)
{
    int i;

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

    printf("Caixas:\n");
    for (i = 0; i < s->config.nCaixas; i++) {
        Caixa *caixa = s->caixas ? s->caixas[i] : NULL;
        if (!caixa) {
            printf("  Caixa %d: indisponivel\n", i + 1);
            continue;
        }
        printf("  Caixa %d: %s | fila=%d | atendidos=%d | receita=%.2f\n",
               caixa->id,
               caixa->ativa ? "ativa" : "inativa",
               TamanhoDaFila(caixa),
               caixa->clientesAtendidos,
               caixa->revenue);
    }

    return 1;
}

void EntradaPessoaSupermercado(ptSupermercado s){
    int x;
    if (s == NULL) return;

    x = Aleatorio(0, 100);
    if (x<s->config.cadenciaEntradaClientes)
    {

    cliente = criarClienteAtivoDoUniverso(&s->universoClientes, s->proximoCliente, s->produtosDisponiveis, s->TotalProdutosDisponiveis, 3);
    if (!cliente) return;

    s->proximoCliente++;
    AdicionarClienteAoSistema(s, cliente);
    printf("Cliente %s entrou no supermercado.\n", cliente->id);
}


int ExecutarSimulacao(ptSupermercado s)
{
    if (s== NULL || s->relogio== NULL ) return 0;
    AvancarRelogio(s->relogio, 1); // Avança o relógio em 1 segundo
    EntradaPessoaSupermercado(s);

    DistribuirClientesParaCaixas(s);

    if (s->caixas != NULL) {
        for (i = 0; i < s->config.nCaixas; i++) {
            ProcessarCaixa(s, s->caixas[i]);
        }
    }

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
            DestruirCaixa(s->caixas[i]);
        }free(s->caixas);
    }
libertarListaClientesAtivos(&s->clientesEmCompras);
libertarUniversoClientes(&s->universoClientes);

free(s);

}