#ifndef SUPERMERCADO_H_INCLUDED
#define SUPERMERCADO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Pessoa.h"
#include "Relogio.h"
//#include "Caixa.h"


typedef struct
{
    int maxEspera;
    int nCaixas;
    int maxFila;
    int minFila;
    int tempoAtendimentoProduto;
    int cadenciaEntradaClientes;
    int horaAbertura;
    int horaFecho;
    //ListaPessoas *LClientes; // Lista das pessoas que andam �s compras
    //ListaProdutos *LProdutos;
    //Hashing       *HCaixas;
    //HoraInicio, HoraFim;
    int CadenciaEntradaClientes;
    Relogio *Rolex;
} CONFIGURACAO, *ptCONFIGURACAO;~

typedef struct
{
    char nome[Supermercado + 1];
    CONFIGURACAO config;
    ptRelogio relogio;
    ptCAIXA caixas;
    int totalCloentesAtendidos;
    int totalPRo
} Supermercado, *ptSupermercado;

Supermercado *CriarSupermercado(char *nome);
int InicializarSupermercado(Supermercado *S, char *config);
int ExecutarSimulacao(Supermercado *S);
void EntradaPessoaSupermercado(Supermercado *S);
int Supermercado_E_Para_Fechar(Supermercado *S);
void DestruirSupermercado(Supermercado *S);


#endif // SUPERMERCADO_H_INCLUDED
