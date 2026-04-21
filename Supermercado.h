#ifndef SUPERMERCADO_H_INCLUDED
#define SUPERMERCADO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Pessoa.h"
#include "Relogio.h"
//#include "Caixa.h"


typedef struct
{   int maxEspera;
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
    
    int CadenciaEntradaClientes;
    Relogio *Rolex;
} CONFIGURACAO, *ptCONFIGURACAO;

typedef struct
{ 
    char nome[50];
    CONFIGURACAO config;
    ptRelogio relogio;    
    //ptCaixa caixas;
    int totalClientesAtendidos;
    int totalProdutosVendidos;
    int totalProdutosOferecidos;
    float custoTotalOfertas;
} Supermercado, *ptSupermercado;


Supermercado *CriarSupermercado(char *nome);
int InicializarSupermercado(ptSupermercado *s, char *nomeFicheiroConfig);
void MostrarSupermercado(ptSupermercado s);
int ExecutarSimulacao(ptSupermercado s);
void EntrarPessoaSupermercado(ptSupermercado s);
float CalcularMediaFilas(ptSupermercado s);
int AbrirSupermercado(ptSupermercado s);
int FecharSupermercado(ptSupermercado s);
int Supermercado_E_Para_Fechar(ptSupermercado s);
void DestruirSupermercado(ptSupermercado s);



#endif 
