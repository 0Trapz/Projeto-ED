#ifndef UTEIS_H
#define UTEIS_H

// Gera um número aleatório entre min e max (inclusive)
int Aleatorio(int min, int max);

// Lê um inteiro do utilizador com mensagem
int LerInteiro(char *txt);

// Converte um char para maiúscula (se for minúscula)
char ToMaiscula(char x);

// Espera um número de milissegundos
void wait(int mlseconds);

// Espera um número de segundos
void wait_segundos(int seconds);

// Verifica se uma tecla foi pressionada
int TeclaPressionada();

#endif // UTEIS_H