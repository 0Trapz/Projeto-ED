#ifndef SUPERMERCADO_H_INCLUDED
#define SUPERMERCADO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Pessoa.h"
#include "Relogio.h"
#include "Caixa.h"

#define MAX_NOME_SUPERMERCADO 50
#define MAX_FUNCIONARIOS 100
#define MAX_NOME_FUNCIONARIO 80

// ------------------------------------------------------------------------------
// Estruturas
// ------------------------------------------------------------------------------

// Estrutura de configuração do supermercado
typedef struct
{   int maxEspera;
    int nCaixas;
    int maxFila;
    int minFila;
    int tempoAtendimentoProduto;
    int cadenciaEntradaClientes;
    int horaAbertura;
    int horaFecho;   
    int maxPreco;
} CONFIGURACAO, *ptCONFIGURACAO;

// Estrutura principal do supermercado
typedef struct
{ 
    char nome[MAX_NOME_SUPERMERCADO + 1];
    CONFIGURACAO config;
    ptRelogio relogio;  
    Produto *produtosDisponiveis;
    int TotalProdutosDisponiveis;
    UniversoClientes universoClientes;
    
    int proximoCliente;

    NodoCliente *clientesEmCompras;

    char funcionarios[MAX_FUNCIONARIOS][MAX_NOME_FUNCIONARIO + 1];
    int funcionarioEmUso[MAX_FUNCIONARIOS];
    int totalFuncionarios;
    int idFuncionario[MAX_FUNCIONARIOS];

    Caixa **caixas;
    NodoCliente *clientesHistorico;

    int totalClientesAtendidos;
    int totalProdutosVendidos;
    int totalProdutosOferecidos;
    float custoTotalOfertas;
    int tempoTotalEspera;
    int numeroTotalEsperas;

} Supermercado, *ptSupermercado;

// ------------------------------------------------------------------------------
// Protótipos de funções
// ------------------------------------------------------------------------------
ptSupermercado CriarSupermercado(char *nome);
int InicializarSupermercado(ptSupermercado s, char *nomeFicheiroConfig);
int CarregarFuncionarios(ptSupermercado s, char *nomeFicheiroFuncionarios);
int ObterFuncionarioLivre(ptSupermercado s);
int AtribuirFuncionarioLivre(ptSupermercado s);
void LibertarFuncionario(ptSupermercado s, int idFuncionario);
int FuncionarioEmUso(ptSupermercado s, int idFuncionario);

void MostrarFuncionarios(ptSupermercado s);
int MostrarSupermercado(ptSupermercado s);

int AbrirCaixaSupermercado(ptSupermercado s);
int FecharCaixaSupermercado(ptSupermercado s);
void PesquisarClienteEmEspera(ptSupermercado s);
void MoverClienteParaOutraCaixa(ptSupermercado s);
int GravarHistoricoSimulacao(ptSupermercado s, char *nomeFicheiro);
void MostrarMemoriaUtilizadaDesperdicada(ptSupermercado s);
void ListarClientesAtendidosPorCaixa(ptSupermercado s);

int ExecutarSimulacao(ptSupermercado s);
int Supermercado_E_Para_Fechar(ptSupermercado s);
int Supermercado_Vazio(ptSupermercado s);
int SimulacaoTerminada(ptSupermercado s);
void DestruirSupermercado(ptSupermercado s);
void EntradaPessoaSupermercado(ptSupermercado s);
int InicializarCaixasSupermercado(ptSupermercado s);

#endif