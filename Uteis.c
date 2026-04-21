#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int Aleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}

int LerInteiro(char *txt) {
    printf("IPV: %s\n", txt);
    int X;
    scanf("%d", &X);
    return X;
}

char ToMaiscula(char x) {
    if ((x >= 'a') && (x <= 'z'))
        return 'A' + x - 'a';
    return x;
}

void wait ( int mlseconds ) {
    clock_t endwait;
    endwait = clock () + mlseconds;
    while (clock() < endwait);
}

void wait_segundos ( int seconds ) {
    wait(seconds * CLOCKS_PER_SEC);
}

int TeclaPressionada() {
    // fazer...
    return 0;
}