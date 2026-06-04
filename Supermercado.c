#include "Supermercado.h"

// Função para aplicar configuração a partir de chave-valor
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

// Função para criar supermercado
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
    s->clientesHistorico = NULL;
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
        s->idFuncionario[i] = -1;
    }


    s->totalClientesAtendidos = 0;
    s->totalProdutosVendidos = 0;
    s->totalProdutosOferecidos = 0;
    s->custoTotalOfertas = 0.0f;
    s->tempoTotalEspera = 0;
    s->numeroTotalEsperas = 0;

    return s;

}

// Função para inicializar supermercado
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

// Função para carregar funcionários
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

// Função para adicionar cliente ao final da lista
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

// Remover primeiro cliente da lista (muda estado 0→1 e retorna ponteiro para Pessoa ou NULL)
// Adicionar cliente à fila de caixa (muda estado 1→1)
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

            if(i == 0){
                s->funcionarioEmUso[i] = 1;
            }
        }

        s->caixas[i] = CriarCaixa(i + 1, operador, operadorID);
        if (!s->caixas[i]) return 0;
        s->caixas[i]->ativa = (i == 0) ? 1 : 0;
    }

    return 1;
}

// Adicionar cliente à fila de caixa (muda estado 1→1)
static void AdicionarClienteAoSistema(ptSupermercado s, Pessoa *cliente)
{
    if (!s || !cliente) return;
    cliente->estado = 0;
    AdicionarClienteAoFim(&s->clientesEmCompras, cliente);
}

// Atualizar clientes em compras (muda estado 1→1)
static void AtualizarClientesEmCompras(ptSupermercado s)
{
    NodoCliente *atual;

    if (s == NULL) return;

    atual = s->clientesEmCompras;

    while (atual != NULL) {
        if (atual->cliente != NULL && atual->cliente->tempoCompraRestante > 0) {
            atual->cliente->tempoCompraRestante--;
        }

        atual = atual->prox;
    }
}

// Distribuir clientes para caixas (muda estado 1→1)
static void DistribuirClientesParaCaixas(ptSupermercado s)
{
    NodoCliente *atual;
    NodoCliente *anterior;
    NodoCliente *remover;
    Pessoa *cliente;
    Caixa *caixa;

    if (!s) return;

    anterior = NULL;
    atual = s->clientesEmCompras;

    while (atual != NULL) {
        cliente = atual->cliente;

        if (cliente != NULL && cliente->tempoCompraRestante <= 0) {
            caixa = CaixaComMenorFila(s->caixas, s->config.nCaixas);

            if (!caixa) {
                caixa = AbrirProximaCaixa(s->caixas, s->config.nCaixas);
                if (!caixa) return;

                if(caixa->id > 0 && caixa->id <= s->totalFuncionarios){
                    s->funcionarioEmUso[caixa->id - 1] = 1; // Marcar operador como ocupado
                }
            }

            if (TamanhoDaFila(caixa) >= s->config.maxFila) {
                Caixa *novaCaixa = AbrirProximaCaixa(s->caixas, s->config.nCaixas);
                if (novaCaixa) {
                    caixa = novaCaixa;

                    if(novaCaixa->id >0 && novaCaixa->id <= s->totalFuncionarios){
                        s->funcionarioEmUso[novaCaixa->id - 1] = 1;
                    }
                    printf("[AUTO] Caixa %d aberta. Operador: %s | Media fila: %.2d\n",
                           novaCaixa->id, novaCaixa->operador, s->config.maxFila);
                }
            }

            remover = atual;

            if (anterior == NULL) {
                s->clientesEmCompras = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }

            atual = atual->prox;

            AdicionarClienteFila(caixa, cliente);
            free(remover);
        } else {
            anterior = atual;
            atual = atual->prox;
        }
    }
}

// Funçao para obter índice do próximo funcionário livre
int ObterFuncionarioLivre(ptSupermercado s)
{
   int i;
   if(s==NULL) return-1;
   for (i=0;i < s->totalFuncionarios;i++)
    if(s->funcionarioEmUso[i] == 0)
     return i; 
   return -1;
}

// Atribuir próximo funcionário livre a caixa
int AtribuirFuncionarioLivre(ptSupermercado s)
{
    int indice = ObterFuncionarioLivre(s);
    if (indice != -1) {
        s->funcionarioEmUso[indice] = 1; // Marcar como ocupado
        return indice;
    }
    return -1; // Nenhum funcionário livre encontrado
    
}

// Liberar funcionário de caixa (muda estado 1→0)
void LibertarFuncionario(ptSupermercado s, int indice)
{
    if (s== NULL) return;
    if (indice <0 || indice >= s->totalFuncionarios) return;
    s->funcionarioEmUso[indice] = 0; // Marcar como livre
}

// Verificar se funcionário está em uso
int FuncionarioEmUso(ptSupermercado s, int idFuncionario)
{
    if (s == NULL) return 0;
    if (idFuncionario < 0 || idFuncionario >= s->totalFuncionarios) return 0;
    return s->funcionarioEmUso[idFuncionario];
}

// Mostrar estado do supermercado
void MostrarFuncionarios(ptSupermercado s)
{
    int i;
    int linhas = 0;
    if (s == NULL) return;

    for (i=0; i <s->totalFuncionarios; i++)
    {
        printf("ID: %d, Nome: %s, Em Uso: %s\n", s->idFuncionario[i], s->funcionarios[i], s->funcionarioEmUso[i] ? "Sim" : "Não");
        
        PausarPagina(&linhas, 20); 
    }
}

// Mostrar estado do supermercado
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
        printf("  Caixa %d: %s | fila=%d | atendidos=%d | receita=%.2f",
               caixa->id,
               caixa->ativa ? "ativa" : "inativa",
               TamanhoDaFila(caixa),
               caixa->clientesAtendidos,
               caixa->revenue);

        if(!caixa->ativa && caixa->motivoFecho[0] != '\0'){
            printf(" | motivo do fecho: %s", caixa->motivoFecho);
        }
        printf("\n");
    }

    return 1;
}

// Funçao para adicionar cliente ao sistema (muda estado 0→1)
void EntradaPessoaSupermercado(ptSupermercado s){
    int x;
    Pessoa *cliente;
    if (s == NULL) return;
    
    if(Supermercado_E_Para_Fechar(s)) return;

    x = Aleatorio(0, 100);
    if (x < s->config.cadenciaEntradaClientes)
    {
       int indiceCliente;
       int numProdutos;
       indiceCliente = Aleatorio(0, s->universoClientes.total - 1);
       numProdutos = Aleatorio(1, MAX_PRODUTOS_CARRINHO);
       cliente = criarClienteAtivoDoUniverso(&s->universoClientes, indiceCliente, s->produtosDisponiveis, s->TotalProdutosDisponiveis, numProdutos); 
       if (!cliente) return;

        s->proximoCliente++;
        AdicionarClienteAoSistema(s, cliente);
        printf("Cliente %s entrou no supermercado.\n", cliente->id);
    }
} 

// Atualizar tempo de espera dos clientes na fila e aplicar ofertas se necessário
static void AtualizarFilaEspera(ptSupermercado s){
    int i; 
    NoCaixa *atual;
    Pessoa *cliente;
    Produto *produtoOferta;

    if( s== NULL || s->caixas == NULL) return;
    for (i=0; i < s->config.nCaixas; i++){
        if (s-> caixas[i] == NULL || s->caixas[i]->fila == NULL)
        continue;

        atual = s->caixas[i]->fila->inicio;
       
        while(atual != NULL){
            cliente = atual->cliente;

            if (cliente !=NULL){
                cliente->tempoEspera++;

                if (cliente->tempoEspera > s->config.maxEspera && cliente->recebeuOferta == 0) {
                    produtoOferta = ObterProdutoMaisBarato(cliente->carrinho, cliente->numProdutos);
                    if (produtoOferta != NULL) {
                        cliente->totalGasto -= produtoOferta->preco;

                        if(cliente->totalGasto <0)
                        cliente->totalGasto = 0;


                    cliente->recebeuOferta = 1;
                    cliente->valorOferta = produtoOferta->preco;
                    cliente->numTotalProdutoOferecido++;
                    
                    s->totalProdutosOferecidos++;
                    s->custoTotalOfertas += produtoOferta->preco;

                        }
                }
            }
            atual = atual->prox;
        }

    }
}

// Gerir caixas automaticamente (abrir se média fila > maxFila, fechar se média fila < minFila)
static void GerirCaixasAutomaticamente(ptSupermercado s){
    int i;
    int caixasAtivas = 0;
    int totalEmFila = 0;
    float mediaFila;
    Caixa *novaCaixa;
    Caixa *caixaFechar = NULL;
    Pessoa *cliente;
    Caixa *destino;

    if (s == NULL || s->caixas == NULL) return;

    for (i = 0; i < s->config.nCaixas; i++) {
        if (s->caixas[i] != NULL && s->caixas[i]->ativa) {
            caixasAtivas++;
            totalEmFila += TamanhoDaFila(s->caixas[i]);
        }
    }

    if (caixasAtivas == 0) return;

    mediaFila = (float)totalEmFila / caixasAtivas;

    if (mediaFila > s->config.maxFila) {
        novaCaixa = AbrirProximaCaixa(s->caixas, s->config.nCaixas);

        if (novaCaixa != NULL) {
            if (novaCaixa->id > 0 && novaCaixa->id <= s->totalFuncionarios) {
                s->funcionarioEmUso[novaCaixa->id - 1] = 1;
            }

            printf("[AUTO] Caixa %d aberta. Operador: %s | Media fila: %.2f\n",
                   novaCaixa->id, novaCaixa->operador, mediaFila);
        }

        return;
    }

    if (mediaFila < s->config.minFila && caixasAtivas > 1) {
        for (i = 0; i < s->config.nCaixas; i++) {
            Caixa *c = s->caixas[i];

            if (c == NULL || !c->ativa) continue;

            if (caixaFechar == NULL || TamanhoDaFila(c) < TamanhoDaFila(caixaFechar)) {
                caixaFechar = c;
            }
        }

        if (caixaFechar == NULL) return;

        caixaFechar->ativa = 0;

        while (TamanhoDaFila(caixaFechar) > 0) {
            cliente = RemoverClienteFila(caixaFechar);

            if (cliente == NULL) break;

            destino = CaixaComMenorFila(s->caixas, s->config.nCaixas);

            if (destino == NULL) {
                AdicionarClienteFila(caixaFechar, cliente);
                caixaFechar->ativa = 1;
                return;
            }

            AdicionarClienteFila(destino, cliente);
        }

        if (caixaFechar->id > 0 && caixaFechar->id <= s->totalFuncionarios) {
            s->funcionarioEmUso[caixaFechar->id - 1] = 0;
        }

        printf("[AUTO] Caixa %d fechada. Operador: %s | Media fila: %.2f\n",
               caixaFechar->id, caixaFechar->operador, mediaFila);
    }
}

// Função para processar caixa e atualizar estatísticas (muda estado 1→1)
int ExecutarSimulacao(ptSupermercado s)
{
    if (s== NULL || s->relogio== NULL ) return 0;
    AvancarRelogio(s->relogio, 1); // Avança o relógio em 1 segundo
    EntradaPessoaSupermercado(s);
    AtualizarClientesEmCompras(s);
    DistribuirClientesParaCaixas(s);
    AtualizarFilaEspera(s);
    GerirCaixasAutomaticamente(s);

    if (s->caixas != NULL) {
        int i;
        for (i = 0; i < s->config.nCaixas; i++) {
            Pessoa *clienteFinalizado = NULL;

            if (ProcessarCaixa(s->caixas[i], &clienteFinalizado) && clienteFinalizado != NULL) {
                s->totalClientesAtendidos++;
                s->totalProdutosVendidos += clienteFinalizado->numProdutos;
                s->tempoTotalEspera += clienteFinalizado->tempoEspera;
                s->numeroTotalEsperas++;
                clienteFinalizado->caixaAtendimento = s->caixas[i]->id;
                clienteFinalizado->totalGastoHistorico += clienteFinalizado->totalGasto;
                clienteFinalizado->totalTempoHistorico += clienteFinalizado->tempoCompra + clienteFinalizado->tempoEspera + clienteFinalizado->tempoCaixa;
                adicionarClienteHistorico(&s->clientesHistorico, clienteFinalizado);
            }
        }
    }

    return 1;
}

// Função para verificar se o supermercado está para fechar
int Supermercado_E_Para_Fechar(Supermercado *s)
{
    int horaAtual;

    if (s == NULL || s->relogio == NULL) return 1;

    horaAtual = s->relogio->horas;

    if (horaAtual >= s->config.horaFecho) {
        return 1;
    }

    return 0;
}

// Função para verificar se o supermercado está vazio (sem clientes em compras e filas vazias)
int Supermercado_Vazio(ptSupermercado s)
{
    int i;
    if (s == NULL) return 1;

    if(s->clientesEmCompras != NULL) {
        return 0;
    }

    if (s->caixas !=NULL){
        for (i=0; i <s->config.nCaixas; i++){
            if(s->caixas[i] == NULL) continue;

            if(TamanhoDaFila(s->caixas[i])>0){
                return 0;
            }

            if (s->caixas[i]->emAtendimento !=NULL){
                return 0;
            }

        }
    }
    return 1;
}

// Função para verificar se a simulação terminou (supermercado para fechar e vazio)
int SimulacaoTerminada(ptSupermercado s)
{
    if (s == NULL) return 1;

    if (Supermercado_E_Para_Fechar(s) && Supermercado_Vazio(s)) {
        return 1; // Simulação terminada
    }
    return 0; // Simulação ainda em andamento
}

// Função para destruir supermercado e liberar memória
void DestruirSupermercado(ptSupermercado s)
{

    int i;
    if (s == NULL) return;
    if (s->relogio != NULL)
    DestruirRelogio(s->relogio);
    
    if (s->caixas != NULL ){
        for (i=0; i < s->config.nCaixas; i++){
            DestruirCaixa(s->caixas[i]);
        }
        free(s->caixas);
    }
    if (s->produtosDisponiveis != NULL)
        free(s->produtosDisponiveis);
    libertarListaClientesAtivos(&s->clientesEmCompras);
    libertarListaClientesHistorico(&s->clientesHistorico);
    libertarUniversoClientes(&s->universoClientes);

    free(s);

}

// Função para abrir uma caixa do supermercado
int AbrirCaixaSupermercado(Supermercado *s)
{
    int numeroCaixa;
    Caixa *caixa;

    if (s == NULL || s->caixas == NULL) return 0;

    MostrarSupermercado(s);

    printf("Numero da caixa a abrir: ");
    if (scanf("%d", &numeroCaixa) != 1) {
        printf("[ERRO] Valor invalido.\n");
        while (getchar() != '\n');
        return 0;
    }
    while (getchar() != '\n');

    if (numeroCaixa < 1 || numeroCaixa > s->config.nCaixas) {
        printf("[ERRO] Caixa invalida.\n");
        return 0;
    }

    caixa = s->caixas[numeroCaixa - 1];

    if (caixa == NULL) {
        printf("[ERRO] Caixa indisponivel.\n");
        return 0;
    }

    if (caixa->ativa) {
        printf("[INFO] Caixa %d ja esta aberta.\n", caixa->id);
        return 0;
    }

    caixa->ativa = 1;
    caixa->motivoFecho[0] = '\0';

    if (caixa->id > 0 && caixa->id <= s->totalFuncionarios) {
        s->funcionarioEmUso[caixa->id - 1] = 1;
    }

    printf("[OK] Caixa %d aberta. Operador: %s\n", caixa->id, caixa->operador);
    return 1;
}


// Função para fechar uma caixa do supermercado e redistribuir clientes (muda estado 1→0)
int FecharCaixaSupermercado(Supermercado *s)
{
    int i;
    int numeroCaixa;
    int totalAtivas = 0;
    int clientesMovidos = 0;
    char motivo[128];

    Caixa *caixaFechar;
    Caixa *destino;
    Pessoa *cliente;

    if (s == NULL || s->caixas == NULL) return 0;

    MostrarSupermercado(s);

    for (i = 0; i < s->config.nCaixas; i++) {
        if (s->caixas[i] != NULL && s->caixas[i]->ativa) {
            totalAtivas++;
        }
    }

    if (totalAtivas <= 1) {
        printf("[INFO] Nao podes fechar a unica caixa ativa.\n");
        return 0;
    }

    printf("Numero da caixa a fechar: ");
    if (scanf("%d", &numeroCaixa) != 1) {
        printf("[ERRO] Valor invalido.\n");
        while (getchar() != '\n');
        return 0;
    }
    while (getchar() != '\n');

    if (numeroCaixa < 1 || numeroCaixa > s->config.nCaixas) {
        printf("[ERRO] Caixa invalida.\n");
        return 0;
    }

    caixaFechar = s->caixas[numeroCaixa - 1];

    if (caixaFechar == NULL) {
        printf("[ERRO] Caixa indisponivel.\n");
        return 0;
    }

    if (!caixaFechar->ativa) {
        printf("[INFO] Caixa %d ja esta fechada.\n", caixaFechar->id);
        return 0;
    }

    printf("Motivo do fecho: ");
    if (fgets(motivo, sizeof(motivo), stdin) == NULL) {
        motivo[0] = '\0';
    }
    motivo[strcspn(motivo, "\r\n")] = '\0';

    caixaFechar->ativa = 0;
    snprintf(caixaFechar->motivoFecho, sizeof(caixaFechar->motivoFecho), "%s", motivo);

    while (TamanhoDaFila(caixaFechar) > 0) {
        cliente = RemoverClienteFila(caixaFechar);

        if (cliente == NULL) break;

        destino = CaixaComMenorFila(s->caixas, s->config.nCaixas);

        if (destino != NULL && TamanhoDaFila(destino) >= s->config.maxFila) {
            Caixa *novaCaixa = AbrirProximaCaixa(s->caixas, s->config.nCaixas);

            if (novaCaixa != NULL) {
                destino = novaCaixa;

                if (novaCaixa->id > 0 && novaCaixa->id <= s->totalFuncionarios) {
                    s->funcionarioEmUso[novaCaixa->id - 1] = 1;
                }

                printf("[AUTO] Caixa %d aberta para redistribuir clientes.\n", novaCaixa->id);
            }
        }

        if (destino == NULL) {
            AdicionarClienteFila(caixaFechar, cliente);
            caixaFechar->ativa = 1;
            caixaFechar->motivoFecho[0] = '\0';
            printf("[ERRO] Nao existe caixa ativa para redistribuir clientes.\n");
            return 0;
        }

        AdicionarClienteFila(destino, cliente);
        clientesMovidos++;
    }

    if (caixaFechar->id > 0 && caixaFechar->id <= s->totalFuncionarios) {
        s->funcionarioEmUso[caixaFechar->id - 1] = 0;
    }

    printf("[OK] Caixa %d fechada. Motivo: %s | Clientes movidos: %d\n",
           caixaFechar->id,
           caixaFechar->motivoFecho,
           clientesMovidos);

    return 1;
}

// Função para mostrar clientes em espera em todas as caixas
static void MostrarClientesEmEspera(Supermercado *s)
{
    int i;
    NoCaixa *atual;

    if (s == NULL || s->caixas == NULL) return;

    printf("\nClientes atualmente em espera:\n");

    for (i = 0; i < s->config.nCaixas; i++) {
        if (s->caixas[i] == NULL || s->caixas[i]->fila == NULL) continue;

        printf("Caixa %d: ", s->caixas[i]->id);

        atual = s->caixas[i]->fila->inicio;

        if (atual == NULL) {
            printf("sem clientes em espera");
        }

        while (atual != NULL) {
            if (atual->cliente != NULL) {
                printf("%s ", atual->cliente->id);
            }
            atual = atual->prox;
        }

        printf("\n");
    }

    printf("\n");
}

// Função para pesquisar cliente em espera por ID e mostrar detalhes
void PesquisarClienteEmEspera(Supermercado *s)
{
    char id[16];
    int i;
    int posicao;
    Pessoa *cliente;

    if (s == NULL || s->caixas == NULL) return;

    MostrarClientesEmEspera(s);

    printf("Digite o ID do cliente para pesquisar: ");
    scanf("%15s", id);

    for (i = 0; i < s->config.nCaixas; i++) {
        if (s->caixas[i] == NULL) continue;

        posicao = 0;
        cliente = ProcurarClienteFila(s->caixas[i], id, &posicao);

        if (cliente != NULL) {
            printf("Cliente %s esta em espera na caixa %d, posicao %d na fila.\n",
                   cliente->id,
                   s->caixas[i]->id,
                   posicao);

            printf("Tempo de espera atual: %d segundos\n",
                   cliente->tempoEspera);

            return;
        }
    }

    printf("[INFO] Cliente %s nao esta em espera em nenhuma caixa.\n", id);
}

// Função para mostrar detalhes dos clientes em espera em todas as caixas
void MostrarClientesEmFila(ptSupermercado s)
{
    int i;
    NoCaixa *atual;
    int encontrou = 0;

    if (s == NULL || s->caixas == NULL) {
        printf("[ERRO] Supermercado invalido.\n");
        return;
    }

    printf("\n========== CLIENTES EM FILA ==========\n");

    for (i = 0; i < s->config.nCaixas; i++) {
        if (s->caixas[i] == NULL || s->caixas[i]->fila == NULL) continue;

        printf("\nCaixa %d (%s) - fila: %d\n",
               s->caixas[i]->id,
               s->caixas[i]->ativa ? "ativa" : "inativa",
               TamanhoDaFila(s->caixas[i]));

        atual = s->caixas[i]->fila->inicio;

        if (atual == NULL) {
            printf("  Sem clientes em fila.\n");
        }

        while (atual != NULL) {
            if (atual->cliente != NULL) {
                printf("  ID: %s | Nome: %s | Espera: %d segundos\n",
                       atual->cliente->id,
                       atual->cliente->nome,
                       atual->cliente->tempoEspera);
                encontrou = 1;
            }

            atual = atual->prox;
        }
    }

    if (!encontrou) {
        printf("\n[INFO] Nao existem clientes em fila neste momento.\n");
    }

    printf("======================================\n");
}

// Função para mover cliente de uma caixa para outra (muda estado 1→1)
void MoverClienteParaOutraCaixa(Supermercado *s){
    char id[16];
    int caixaDestino;
    int i;
    int origem = -1;
    Pessoa *cliente;
    Caixa *destino;
    if (s == NULL || s->caixas == NULL) return;

     MostrarClientesEmEspera(s);

     printf("Digite o ID do cliente para mover: ");
     scanf("%15s", id);

     printf("Numero da caixa de destino: ");
     scanf("%d", &caixaDestino);

    if(caixaDestino < 1 || caixaDestino > s->config.nCaixas){
      printf("[ERRO] Caixa de destino inválida.\n");
      return;
    }
    destino = s->caixas[caixaDestino - 1];
    if (destino == NULL || destino->ativa == 0 ){
      printf("[ERRO] Caixa de destino não está ativa.\n");
      return;
    }

    for(i=0; i < s->config.nCaixas; i++){
        if (s->caixas[i] == NULL) continue;
        
        cliente = ProcurarClienteFila(s->caixas[i], id, NULL);

        if (cliente != NULL) {
            origem = i;
            break;
        }
    }
    if (origem == -1) {
    printf("[INFO] Cliente %s não foi encontrado em nenhuma fila de espera.\n", id);
    return;
   }

   if (origem == caixaDestino - 1) {
    printf("[INFO] Cliente %s já está nessa caixa %d.\n", id, caixaDestino);
    return;
   }
   cliente = RemoverClienteFilaPorID(s->caixas[origem], id);

    if (cliente == NULL) {
     printf("[ERRO] Falha ao remover cliente %s da fila.\n", id);
     return;
    }
    
    AdicionarClienteFila(destino, cliente);
    printf("[OK] Cliente %s movido da caixa %d para a caixa %d.\n", id, s->caixas[origem]->id, destino->id);
}

// Função para gravar histórico da simulação em arquivo CSV
int GravarHistoricoSimulacao(Supermercado *s, char *nomeFicheiro){
     FILE *f;
    NodoCliente *atual;
    float tempoMedioEspera = 0.0f;

    if (s == NULL || nomeFicheiro == NULL) return 0;

    f = fopen(nomeFicheiro, "w");

    if (f == NULL) {
        printf("[ERRO] Nao foi possivel criar o ficheiro %s.\n", nomeFicheiro);
        return 0;
    }

    if (s->numeroTotalEsperas > 0) {
        tempoMedioEspera = (float)s->tempoTotalEspera / s->numeroTotalEsperas;
    }

    fprintf(f, "ESTATISTICAS\n");
    fprintf(f, "clientes_atendidos;%d\n", s->totalClientesAtendidos);
    fprintf(f, "produtos_vendidos;%d\n", s->totalProdutosVendidos);
    fprintf(f, "produtos_oferecidos;%d\n", s->totalProdutosOferecidos);
    fprintf(f, "custo_total_ofertas;%.2f\n", s->custoTotalOfertas);
    fprintf(f, "tempo_medio_espera;%.2f\n", tempoMedioEspera);

    fprintf(f, "\nCLIENTES_ATENDIDOS\n");
    fprintf(f, "id;nome;caixa_atendimento;num_produtos;countVezesIda;total_gasto;tempo_compra;tempo_espera;tempo_caixa;recebeu_oferta;valor_oferta\n");

    atual = s->clientesHistorico;

    while (atual != NULL) {
        Pessoa *p = atual->cliente;

        if (p != NULL) {
            fprintf(f, "%s;%s;%d;%d;%d;%.2f;%.2f;%d;%.2f;%d;%.2f\n",
                p->id,
                p->nome,
                p->caixaAtendimento,
                p->numProdutos,
                p->countVezesIda,
                p->totalGasto,
                p->tempoCompra,
                p->tempoEspera,
                p->tempoCaixa,
                p->recebeuOferta,
                p->valorOferta);
        }

        atual = atual->prox;
    }

    fclose(f);

    printf("[OK] Historico gravado em %s.\n", nomeFicheiro);
    return 1;
}

// Função para mostrar memória utilizada e desperdiçada (aproximada)
void MostrarMemoriaUtilizadaDesperdicada(Supermercado *s){
    int i;
    size_t memoriaUsada = 0;
    size_t memoriaDesperdicada = 0;
    NodoCliente *atual;

    if (s == NULL) return;

    memoriaUsada += sizeof(Supermercado);

    if (s->produtosDisponiveis != NULL) {
        memoriaUsada += sizeof(Produto) * s->TotalProdutosDisponiveis;
    }

    if (s->universoClientes.array != NULL) {
        memoriaUsada += sizeof(Pessoa) * s->universoClientes.total;
    }

    if (s->caixas != NULL) {
        memoriaUsada += sizeof(Caixa *) * s->config.nCaixas;

        for (i = 0; i < s->config.nCaixas; i++) {
            Caixa *c = s->caixas[i];

            if (c != NULL) {
                memoriaUsada += sizeof(Caixa);

                if (c->fila != NULL) {
                    NoCaixa *no = c->fila->inicio;

                    memoriaUsada += sizeof(FilaCaixa);

                    while (no != NULL) {
                        memoriaUsada += sizeof(NoCaixa);
                        no = no->prox;
                    }
                }

                if (c->operador != NULL) {
                    memoriaUsada += strlen(c->operador) + 1;
                }

                if (!c->ativa) {
                    memoriaDesperdicada += sizeof(Caixa);

                    if (c->fila != NULL) {
                        memoriaDesperdicada += sizeof(FilaCaixa);
                    }
                }
            }
        }
    }

    atual = s->clientesEmCompras;

    while (atual != NULL) {
    memoriaUsada += sizeof(NodoCliente);

    if (atual->cliente != NULL && atual->cliente->numProdutos < MAX_PRODUTOS_CARRINHO) {
        memoriaDesperdicada += (size_t)(MAX_PRODUTOS_CARRINHO - atual->cliente->numProdutos) * sizeof(Produto);
    }

    atual = atual->prox;
}


    atual = s->clientesHistorico;

    while (atual != NULL) {
        memoriaUsada += sizeof(NodoCliente);
        if (atual->cliente != NULL && atual->cliente->numProdutos < MAX_PRODUTOS_CARRINHO) {
            memoriaDesperdicada += (size_t)(MAX_PRODUTOS_CARRINHO - atual->cliente->numProdutos) * sizeof(Produto);
        }
        atual = atual->prox;
    }

    printf("\n========== MEMORIA ==========\n");
    printf("Memoria usada aproximada: %zu bytes\n", memoriaUsada);
    printf("Memoria desperdicada aproximada: %zu bytes\n", memoriaDesperdicada);
    printf("=============================\n\n");
}

// Função para listar clientes atendidos por caixa e mostrar detalhes de cada cliente
void ListarClientesAtendidosPorCaixa(ptSupermercado s)
{
    int numeroCaixa;
    int encontrados = 0;
    int linhas = 0;
    NodoCliente *atual;

    if (s == NULL) return;

    printf("Numero da caixa: ");
    scanf("%d", &numeroCaixa);

    if (numeroCaixa < 1 || numeroCaixa > s->config.nCaixas) {
        printf("[ERRO] Caixa invalida.\n");
        return;
    }

    printf("\nClientes atendidos pela caixa %d:\n", numeroCaixa);
    printf("----------------------------------------\n");

    atual = s->clientesHistorico;

    while (atual != NULL) {
        Pessoa *p = atual->cliente;

        if (p != NULL && p->caixaAtendimento == numeroCaixa) {
            printf("ID: %s | Nome: %s | Produtos: %d | Total: %.2f | Espera: %d\n",
                   p->id,
                   p->nome,
                   p->numProdutos,
                   p->totalGasto,
                   p->tempoEspera);
            encontrados++;

            PausarPagina(&linhas, 20);
        }

        atual = atual->prox;
    }

    if (encontrados == 0) {
        printf("[INFO] Nenhum cliente encontrado para esta caixa.\n");
    }

    printf("----------------------------------------\n");
    printf("Total encontrados: %d\n\n", encontrados);
}