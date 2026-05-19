#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Supermercado.h"
#include "Uteis.h"


void ExecutarCicloSimulacao(ptSupermercado Lidl)
{
    int terminar = 0;
    int iteracoes = 0;
    int linhas = 0; 

    if (Lidl == NULL) return;

    if (SimulacaoTerminada(Lidl)) {
        printf("[INFO] Simulacao terminada. Supermercado fechado e vazio.\n");
        return;
    }

    printf("==========================================================\n");
    printf("        A Executar Simulacao\n");
    printf("==========================================================\n\n");


    printf("[INFO] Simulacao iniciada. Hora atual: %02d:%02d:%02d\n",
           Lidl->relogio->horas,
           Lidl->relogio->minutos,
           Lidl->relogio->segundos);
        

    while (!terminar && iteracoes < 3600)
    {
        if (!ExecutarSimulacao(Lidl)) {
            printf("[ERRO] Erro ao executar simulacao.\n");
            break;
        }

        iteracoes++;
        terminar = SimulacaoTerminada(Lidl);

        if (iteracoes % 600 == 0){
            PausarPagina(&linhas, 1);
        }
        
    }

    printf("[INFO] Simulacao terminada apos %d iteracoes.\n", iteracoes);

    if (Supermercado_E_Para_Fechar(Lidl) && !Supermercado_Vazio(Lidl)) {
        printf("[INFO] Supermercado fechado a novos clientes. A esvaziar caixas...\n");
    }

    printf("[PAUSA] Simulacao pausada apos %d segundos.\n", iteracoes);
    printf("[INFO] Hora atual: %02d:%02d:%02d\n",
           Lidl->relogio->horas,
           Lidl->relogio->minutos,
           Lidl->relogio->segundos);
}


void MostrarEstatisticasFinais(Supermercado *Lidl)
{
    float lucroTotal = 0.0f;
    float tempoMedioEspera = 0.0f;
    int i;

    Caixa *caixaMaisClientes = NULL;
    Caixa *caixaMaisProdutos = NULL;
    Caixa *caixaMenosClientes = NULL;

    if (Lidl == NULL) return;

    if (Lidl->numeroTotalEsperas > 0) {
        tempoMedioEspera = (float)Lidl->tempoTotalEspera / Lidl->numeroTotalEsperas;
    }

    if (Lidl->caixas != NULL) {
        for (i = 0; i < Lidl->config.nCaixas; i++) {
            Caixa *c = Lidl->caixas[i];

            if (c == NULL) continue;

            lucroTotal += c->revenue;

            if (caixaMaisClientes == NULL || c->clientesAtendidos > caixaMaisClientes->clientesAtendidos) {
                caixaMaisClientes = c;
            }

            if (caixaMaisProdutos == NULL || c->produtosVendidos > caixaMaisProdutos->produtosVendidos) {
                caixaMaisProdutos = c;
            }

            if (c->clientesAtendidos > 0) {
                if (caixaMenosClientes == NULL || c->clientesAtendidos < caixaMenosClientes->clientesAtendidos) {
                    caixaMenosClientes = c;
                }
            }
        }
    }

    printf("\n==========================================================\n");
    printf("           ESTATISTICAS DO SUPERMERCADO\n");
    printf("==========================================================\n");
    printf("  - Clientes atendidos: %d\n", Lidl->totalClientesAtendidos);
    printf("  - Produtos vendidos: %d\n", Lidl->totalProdutosVendidos);
    printf("  - Produtos oferecidos: %d\n", Lidl->totalProdutosOferecidos);
    printf("  - Custo total ofertas: %.2f\n", Lidl->custoTotalOfertas);
    printf("  - Total de Faturado nas caixas: %.2f\n", lucroTotal);
    printf("  - Tempo medio de espera: %.2f segundos\n", tempoMedioEspera);

    if (caixaMaisClientes != NULL) {
        printf("  - Caixa que atendeu mais pessoas: Caixa %d (%d clientes)\n",
               caixaMaisClientes->id,
               caixaMaisClientes->clientesAtendidos);
    }

    if (caixaMaisProdutos != NULL) {
        printf("  - Caixa que vendeu mais produtos: Caixa %d (%d produtos)\n",
               caixaMaisProdutos->id,
               caixaMaisProdutos->produtosVendidos);
    }

    if (caixaMenosClientes != NULL) {
        printf("  - Operador que atendeu menos pessoas: %s (%d clientes, caixa %d)\n",
               caixaMenosClientes->operador,
               caixaMenosClientes->clientesAtendidos,
               caixaMenosClientes->id);
    }

    printf("==========================================================\n\n");
}

int main()
{
    printf("\n");
    printf("==========================================================\n");
    printf("        SIMULADOR DE SUPERMERCADO - ED 25-26\n");
    printf("==========================================================\n");
    printf("\n");

    srand(time(NULL));

    // ========== CRIAR SUPERMERCADO ==========
    printf("[INIT] A criar supermercado...\n");
    Supermercado *Lidl = CriarSupermercado("Lidl");
    if (!Lidl) {
        printf("[ERRO] Falha ao criar o supermercado.\n");
        return 1;
    }
    printf("[OK] Supermercado criado com sucesso.\n\n");

    // ========== CARREGAR CONFIGURACAO ==========
    printf("[INIT] A carregar configuracao (config.txt)...\n");
    if (!InicializarSupermercado(Lidl, "config.txt")) {
        printf("[ERRO] Falha ao inicializar supermercado - verifique config.txt.\n");
        DestruirSupermercado(Lidl);
        return 1;
    }

    // Validar configurações críticas
    if (Lidl->config.nCaixas <= 0) {
        printf("[ERRO] Numero de caixas invalido: %d\n", Lidl->config.nCaixas);
        DestruirSupermercado(Lidl);
        return 1;
    }
    if (Lidl->config.maxFila <= 0) {
        printf("[ERRO] Max fila invalido: %d\n", Lidl->config.maxFila);
        DestruirSupermercado(Lidl);
        return 1;
    }

    printf("[OK] Configuracao carregada:\n");
    printf("     - Numero de caixas: %d\n", Lidl->config.nCaixas);
    printf("     - Hora abertura: %d:00\n", Lidl->config.horaAbertura);
    printf("     - Hora fecho: %d:00\n", Lidl->config.horaFecho);
    printf("     - Max fila: %d clientes\n", Lidl->config.maxFila);
    printf("     - Cadencia entrada: %d%%\n\n", Lidl->config.cadenciaEntradaClientes);

    // ========== CARREGAR FUNCIONARIOS ==========
    printf("[INIT] A carregar funcionarios (funcionarios.txt)...\n");
    if (!CarregarFuncionarios(Lidl, "funcionarios.txt")) {
        printf("[AVISO] Nao foi possivel carregar funcionarios - usando operadores genericos.\n");
    } else {
        printf("[OK] Carregados %d funcionarios.\n\n", Lidl->totalFuncionarios);
    }

    // ========== CARREGAR PRODUTOS ==========
    printf("[INIT] A carregar produtos (produtos.txt)...\n");
    Produto produtosTemp[MAX_PRODUTOS_FICHEIRO];
    int totalProdutos = CarregarProdutosDeFicheiro("produtos.txt", produtosTemp, MAX_PRODUTOS_FICHEIRO);
    
    if (totalProdutos <= 0) {
        printf("[ERRO] Falha ao carregar produtos ou ficheiro vazio.\n");
        DestruirSupermercado(Lidl);
        return 1;
    }

    Lidl->produtosDisponiveis = (Produto *)malloc(sizeof(Produto) * (size_t)totalProdutos);
    if (!Lidl->produtosDisponiveis) {
        printf("[ERRO] Falha ao reservar memoria para produtos.\n");
        DestruirSupermercado(Lidl);
        return 1;
    }

    memcpy(Lidl->produtosDisponiveis, produtosTemp, sizeof(Produto) * (size_t)totalProdutos);
    Lidl->TotalProdutosDisponiveis = totalProdutos;
    printf("[OK] Carregados %d produtos.\n\n", totalProdutos);

    // ========== CARREGAR CLIENTES ==========
    printf("[INIT] A carregar universo de clientes (clientes.txt)...\n");
    Lidl->universoClientes = carregarUniversoClientes("clientes.txt");
    
    if (Lidl->universoClientes.array == NULL || Lidl->universoClientes.total <= 0) {
        printf("[ERRO] Falha ao carregar clientes ou ficheiro vazio.\n");
        DestruirSupermercado(Lidl);
        return 1;
    }
    printf("[OK] Carregados %d clientes no universo.\n\n", Lidl->universoClientes.total);

    // ========== INICIALIZAR CAIXAS ==========
    printf("[INIT] A inicializar caixas...\n");
    if (!InicializarCaixasSupermercado(Lidl)) {
        printf("[ERRO] Falha ao inicializar caixas.\n");
        DestruirSupermercado(Lidl);
        return 1;
    }
    printf("[OK] Inicializadas %d caixas (1 ativa, resto inativa).\n\n", Lidl->config.nCaixas);

    
    int opcao = -1;
    do{
        printf("\n==============================Menu==============================\n");
        printf("1 - Mostrar Supermercado\n");
        printf("2 - Mostrar Funcionarios\n");
        printf("3 - Realizar a simulacao de 1H\n");
        printf("4 - Mostrar estatisticas finais\n");
        printf("5 - Abrir caixa\n");
        printf("6 - Fechar caixa\n");
        printf("7 - Pesquisar cliente em espera\n");
        printf("8 - Mover cliente para outra caixa\n");
        printf("9 - Gravar Historico de Simulacao\n");
        printf("10 - Mostrar memoria utilizada/desperdicada\n");
        printf("11 - Listar clientes atendidos por caixa\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Opcao invalida. Tente novamente.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (opcao) {
            case 1: RegistarAcaoMenuCSV("historico_menu.csv", opcao, "Mostrar Supermercado"); 
                break;
            case 2: RegistarAcaoMenuCSV("historico_menu.csv", opcao, "Mostrar Funcionarios"); 
                break;
            case 3: RegistarAcaoMenuCSV("historico_menu.csv", opcao, "Realizar a simulacao de 1H"); 
                break;
            case 4: RegistarAcaoMenuCSV("historico_menu.csv", opcao, "Mostrar estatisticas finais"); 
                break;
            case 5: RegistarAcaoMenuCSV("historico_menu.csv", opcao, "Abrir caixa"); 
                break;
            case 6: RegistarAcaoMenuCSV("historico_menu.csv", opcao, "Fechar caixa"); 
                break;
            case 7: RegistarAcaoMenuCSV("historico_menu.csv", opcao, "Pesquisar cliente em espera"); 
                break;
            case 8: RegistarAcaoMenuCSV("historico_menu.csv", opcao, "Mover cliente para outra caixa"); 
                break;
            case 9: RegistarAcaoMenuCSV("historico_menu.csv", opcao, "Gravar Historico de Simulacao"); 
                break;
            case 10: RegistarAcaoMenuCSV("historico_menu.csv", opcao, "Mostrar memoria utilizada/desperdicada"); 
                break;
            case 11: RegistarAcaoMenuCSV("historico_menu.csv", opcao, "Listar clientes atendidos por caixa"); 
                break;
            case 0: RegistarAcaoMenuCSV("historico_menu.csv", opcao, "Sair");
                break;
            default: RegistarAcaoMenuCSV("historico_menu.csv", opcao, "Opcao invalida");
                break;
        }
        
        switch(opcao){
            case 1:
                MostrarSupermercado(Lidl);
                break;
            case 2:
                MostrarFuncionarios(Lidl);
                
                break;
            case 3:
                ExecutarCicloSimulacao(Lidl);
                
                break;
            case 4:
                MostrarEstatisticasFinais(Lidl);
                break;
            case 5:
                AbrirCaixaSupermercado(Lidl);
                break;
            case 6:
                FecharCaixaSupermercado(Lidl);
                break;
            case 7:
                PesquisarClienteEmEspera(Lidl);
                break;
            case 8:
                MoverClienteParaOutraCaixa(Lidl);
                break;
            case 9:
                GravarHistoricoSimulacao(Lidl, "historico_simulacao.csv");
                break;
            case 10:
                MostrarMemoriaUtilizadaDesperdicada(Lidl);
                break;
            case 11:
                ListarClientesAtendidosPorCaixa(Lidl);
                
                break;
            case 0:
                printf("A sair do programa...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    }while (opcao != 0);
    
    
    DestruirSupermercado(Lidl);

    printf("[INFO] Supermercado destruido. Programa terminado.\n");
    return 0;
}
