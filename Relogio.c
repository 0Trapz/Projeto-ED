
#include "Relogio.h"

Relogio *CriarRelogio(int _velocidade)
{
    Relogio *R = (Relogio *)malloc(sizeof(Relogio));
    //R->Tinicio =
    R->Velocidade = _velocidade;
    return R;
}
void DestruirRelogio(Relogio *R)
{
    free(R);
}
time_t GetTempo(Relogio *R)
{
    //return R->Tinicio + R->Velocidade * (Agora - R->Tinicio);
    return 0;
}
