#ifndef CARTA_H
#define CARTA_H

#include "ladocarta.h"
#include <string>

class Carta {
private:
    LadoCarta* ladoClaro;
    LadoCarta* ladoOscuro;
    bool estaVolteada;

public:
    Carta(LadoCarta* claro);
    Carta(LadoCarta* claro, LadoCarta* oscuro);
    Carta(const Carta& cartaCopia);
    Carta& operator=(const Carta& cartaCopia);
    Carta();
    ~Carta();

    LadoCarta* getLadoActivo() const;
    void voltear();
    bool esFlip() const;
    bool esValida()const;
};

#endif // CARTA_H
