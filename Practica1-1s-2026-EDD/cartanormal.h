#ifndef CARTANORMAL_H
#define CARTANORMAL_H
#include "carta.h"
class CartaNormal:public Carta
{
private:
    LadoCarta frente;
public:
    CartaNormal(LadoCarta frontal);

    const LadoCarta& getLadoActivo()const override;
    void voltear()override;
    bool esFlip()const override;
};

#endif // CARTANORMAL_H
