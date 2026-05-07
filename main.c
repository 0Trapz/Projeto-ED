#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Supermercado.h"
#include "Uteis.h"

int main()
{
    printf("\n");
    printf("==========================================================\n");
    printf("        SIMULADOR DE SUPERMERCADO - ED 25-26\n");
    printf("==========================================================\n");
    printf("\n");

    srand(time(NULL));

    // ========== CRIAR SUPERMERCADO ==========
    printf("[INIT] Criando supermercado...\n");
    Supermercado *Lidl = CriarSupermercado("Lidl");
    if (!Lidl) {
        printf("[ERRO] Falha ao criar o supermercado.\n");
        return 1;
    }
    printf("[OK] Supermercado criado com sucesso.\n\n");

    // ========== CARREGAR CONFIGURACAO ==========
    printf("[INIT] Carregando configuracao (config.txt)...\n");
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
    printf("[INIT] Carregando funcionarios (funcionarios.txt)...\n");
    if (!CarregarFuncionarios(Lidl, "funcionarios.txt")) {
        printf("[AVISO] Nao foi possivel carregar funcionarios - usando operadores genericos.\n");
    } else {
        printf("[OK] Carregados %d funcionarios.\n\n", Lidl->totalFuncionarios);
    }

    // ========== CARREGAR PRODUTOS ==========
    printf("[INIT] Carregando produtos (produtos.txt)...\n");
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
    printf("[INIT] Carregando universo de clientes (clientes.txt)...\n");
    Lidl->universoClientes = carregarUniversoClientes("clientes.txt");
    
    if (Lidl->universoClientes.array == NULL || Lidl->universoClientes.total <= 0) {
        printf("[ERRO] Falha ao carregar clientes ou ficheiro vazio.\n");
        DestruirSupermercado(Lidl);
        return 1;
    }
    printf("[OK] Carregados %d clientes no universo.\n\n", Lidl->universoClientes.total);

    // ========== INICIALIZAR CAIXAS ==========
    printf("[INIT] Inicializando caixas...\n");
    if (!InicializarCaixasSupermercado(Lidl)) {
        printf("[ERRO] Falha ao inicializar caixas.\n");
        DestruirSupermercado(Lidl);
        return 1;
    }
    printf("[OK] Inicializadas %d caixas (1 ativa, resto inativa).\n\n", Lidl->config.nCaixas);

    // ========== INICIAR SIMULACAO ==========
    printf("==========================================================\n");
    printf("        SIMULACAO EM ANDAMENTO\n");
    printf("        (Prima ESC para menu, Q para sair)\n");
    printf("==========================================================\n\n");

    int Terminar = 0;
    int iteracoes = 0;

    while (!Terminar && iteracoes < 1440)  // 1440 minutos = 24 horas
    {
        // Executar um passo da simulacao
        if (!ExecutarSimulacao(Lidl)) {
            printf("[ERRO] Erro ao executar simulacao.\n");
            break;
        }

        // Imprimir estatísticas a cada 60 iterações
        if (iteracoes % 60 == 0 && iteracoes > 0) {
            printf("[%03d:%02d] Atendidos: %d | Proximos a entrar: %d\n",
                   Lidl->relogio->horas, Lidl->relogio->minutos,
                   Lidl->totalClientesAtendidos,
                   Lidl->proximoCliente);
        }

        // Simular passagem de tempo
        // wait_segundos(1);  // Desabilitar para testes - um segundo por iteração é muito lento
        iteracoes++;

        // Verificar se deve fechar
        Terminar = Supermercado_E_Para_Fechar(Lidl);
    }

    // ========== FINALIZACAO ==========
    printf("\n==========================================================\n");
    printf("           SIMULACAO FINALIZADA\n");
    printf("==========================================================\n");
    printf("\nEstatisticas finais:\n");
    printf("  - Iteracoes executadas: %d\n", iteracoes);
    printf("  - Clientes atendidos: %d\n", Lidl->totalClientesAtendidos);
    printf("  - Produtos vendidos: %d\n", Lidl->totalProdutosVendidos);
    printf("  - Custo total ofertas: %.2f\n", Lidl->custoTotalOfertas);
    printf("  - Total de Lucro das caixas: %.2f\n", Lidl->caixas ? (Lidl->caixas[0]->revenue + (Lidl->config.nCaixas > 1 ? Lidl->caixas[1]->revenue : 0)) : 0.0f);
    printf("==========================================================\n\n");

    // Limpeza
    DestruirSupermercado(Lidl);

    printf("[INFO] Supermercado destruido. Programa terminado.\n");
    return 0;
}