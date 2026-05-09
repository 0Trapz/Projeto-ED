#include "Uteis.h"
#include <string.h>

// Gera um número aleatório entre min e max (inclusive)
int Aleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Lê um inteiro do utilizador com mensagem
int LerInteiro(char *txt) {
    printf("IPV: %s\n", txt);
    int X;
    scanf("%d", &X);
    return X;
}

// Converte um char para maiúscula (se for minúscula)
char ToMaiscula(char x) {
    if ((x >= 'a') && (x <= 'z'))
        return 'A' + x - 'a';
    return x;
}

// Função para esperar um número específico de milissegundos
void wait ( int mlseconds ) {
    clock_t endwait;
    endwait = clock () + mlseconds;
    while (clock() < endwait);
}

// Função para esperar um número específico de segundos
void wait_segundos ( int seconds ) {
    wait(seconds * CLOCKS_PER_SEC);
}

// Verifica se há uma tecla pendente sem bloquear o ciclo principal
int TeclaPressionada() {
    if (!_kbhit()) {
        return 0;
    }

    while (_kbhit()) {
        (void)_getch();
    }

    return 1;
}

// Função utilitária para ler uma linha de um ficheiro para buffer, removendo '\n'/'\r\n'.
int LerLinhaFicheiro(FILE *f, char *destino, int tamanho) {
    if (!f || !destino || tamanho <= 1) return 0;
    if (!fgets(destino, tamanho, f)) return 0;
    destino[strcspn(destino, "\r\n")] = '\0';
    return 1;
}

void RegistarAcaoMenuCSV(const char *ficheiro, int opcao, const char *descricao) {

    FILE *f;

    if (ficheiro == NULL || descricao == NULL) return;

    f = fopen(ficheiro, "a");

    if (f == NULL) return;
    fprintf(f, "%d;%s\n", opcao, descricao);
    fclose(f);
}