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
    ~Carta();

    // Métodos Puros (El contrato que los hijos deben cumplir)
    LadoCarta* getLadoActivo() const;
    void voltear();
    // Para saber si es carta especial sin cast
    bool esFlip() const;
};

#endif // CARTA_H
