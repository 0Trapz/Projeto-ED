#ifndef UTEIS_H
#define UTEIS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

// ------------------------------------------------------------------------------
// Protótipos de funções
// ------------------------------------------------------------------------------
int Aleatorio(int min, int max);
int LerInteiro(char *txt);
char ToMaiscula(char x);
void wait(int mlseconds);
void wait_segundos(int seconds);
int TeclaPressionada();
int LerLinhaFicheiro(FILE *f, char *destino, int tamanho);
void RegistarAcaoMenuCSV(const char *ficheiro, int opcao, const char *descricao);
void PausarPagina(int *contador, int limite);

#endif // UTEIS_H