#ifndef Relogio_H_INCLUDED
#define Relogio_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

// ------------------------------------------------------------------------------
// Estrutura que representa um relógio
// ------------------------------------------------------------------------------
typedef struct 
{
    int horas;
    int minutos;
    int segundos;
} Relogio, *ptRelogio; 

// ------------------------------------------------------------------------------
// Protótipos de funções
// ------------------------------------------------------------------------------
ptRelogio CriarRelogio(int h, int m, int s);
void DestruirRelogio(ptRelogio r);
void MostrarRelogio(ptRelogio r);
int AcertarRelogio(ptRelogio r, int h, int m, int s);
void AvancarRelogio(ptRelogio r, int segundos);
int ObterSegundosRelogio(ptRelogio r);

#endif 