#ifndef RELOGIO_H_INCLUDED
#define RELOGIO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    time_t Tinicio;
    int Velocidade;
}Relogio;
Relogio *CriarRelogio(int _velocidade);
void DestruirRelogio(Relogio *R);
time_t GetTempo(Relogio *R);

#endif // RELOGIO_H_INCLUDED
