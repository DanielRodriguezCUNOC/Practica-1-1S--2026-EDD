#ifndef CARTA_H
#define CARTA_H

enum Color {ROJO, AMARILLO,AZUL,VERDE,NEGRO, NARANJA,ROSA, PURPURA, TURQUESA};


class Carta
{
public:
    Carta(Color color);
    virtual ~Carta();
    Color getColor();
    virtual void aplicarEfecto();

private:
    Color color;

};

#endif // CARTA_H
