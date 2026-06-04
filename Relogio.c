#include "Relogio.h"

// Função auxiliar para validar a hora  
static int HoraValida(int h, int m, int s){
    if(h < 0 || h > 23) return 0;
    if(m < 0 || m > 59) return 0;
    if(s < 0 || s > 59) return 0;
    return 1;
}

// Implementação das funções do Relógio
ptRelogio CriarRelogio(int h, int m, int s){
    if (!HoraValida(h,m,s)) return NULL;
    ptRelogio r = (ptRelogio) malloc(sizeof(Relogio));    
    if (!r)return NULL;
    r->horas =h;
    r->minutos = m;
    r->segundos = s;
    return r;
}

// Função para destruir o relógio e liberar a memória alocada
void DestruirRelogio(ptRelogio r){
    if (r) free(r);
}

// Função para mostrar a hora atual do relógio
void MostrarRelogio(ptRelogio r){
    if (!r) return;
    printf("%02d:%02d:%02d\n", r->horas, r->minutos, r->segundos);
}

// Função para acertar o relógio com uma nova hora, minuto e segundo
int AcertarRelogio(ptRelogio r, int h, int m, int s){
    if (!r) return 0;
    if (!HoraValida(h, m, s)) return 0;
    r->horas = h;
    r->minutos = m;
    r->segundos = s;
    return 1;
}

// Função para obter o total de segundos representados pelo relógio
int ObterSegundosRelogio(ptRelogio r){
    if (!r) return -1;
    return r->horas * 3600 + r->minutos * 60 + r->segundos;
}

// Função para avançar o relógio em um número específico de segundos
void AvancarRelogio(ptRelogio r, int segundos){
    int total;
    if(!r || segundos < 0) return;
    total = ObterSegundosRelogio(r);
    total += segundos;
    total = total % (24*3600);
    r->horas = total / 3600;
    total = total % 3600;
    r->minutos = total /60;
    r->segundos = total % 60; 
}