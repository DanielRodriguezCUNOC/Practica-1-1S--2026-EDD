#ifndef NODOCARTA_H
#define NODOCARTA_H
#include "carta.h"

class NodoCarta
{
public:
    Carta* carta;
    NodoCarta* siguienteCarta;
    NodoCarta* anteriorCarta;
    NodoCarta(Carta* carta);
    ~NodoCarta();
};

#endif // NODOCARTA_H
