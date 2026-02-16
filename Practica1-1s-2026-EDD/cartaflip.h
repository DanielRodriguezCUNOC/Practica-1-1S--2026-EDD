#ifndef CARTAFLIP_H
#define CARTAFLIP_H
#include "carta.h"
class CartaFlip:public Carta
{

private:
    LadoCarta ladoClaro;
    LadoCarta ladoOscuro;
    bool estaVolteada;

public:
    CartaFlip(LadoCarta claro, LadoCarta oscuro);
    const LadoCarta& getLadoActivo()const override;
    void voltear()override;
    bool esFlip()const override;
};

#endif // CARTAFLIP_H
