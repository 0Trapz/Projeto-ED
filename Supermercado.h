#ifndef SUPERMERCADO_H_INCLUDED
#define SUPERMERCADO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "Pessoa.h"
#include "Relogio.h"

typedef struct
{
    char *NOME;
    //ListaPessoas *LClientes; // Lista das pessoas que andam às compras
    //ListaProdutos *LProdutos;
    //Hashing       *HCaixas;
    //HoraInicio, HoraFim;
    int CadenciaEntradaClientes;
    Relogio *Rolex;
}Supermercado;

Supermercado *CriarSupermercado(char *nome);
int InicializarSupermercado(Supermercado *S, char *config);
int ExecutarSimulacao(Supermercado *S);
void EntradaPessoaSupermercado(Supermercado *S);
int Supermercado_E_Para_Fechar(Supermercado *S);
void DestruirSupermercado(Supermercado *S);


#endif // SUPERMERCADO_H_INCLUDED
