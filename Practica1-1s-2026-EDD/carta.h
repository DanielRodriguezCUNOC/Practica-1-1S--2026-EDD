#ifndef CARTA_H
#define CARTA_H

#include "ladocarta.h"
#include <string>

class Carta {

public:
    virtual ~Carta();
    // Métodos Puros (El contrato que los hijos deben cumplir)
    virtual const LadoCarta& getLadoActivo() const = 0;
    virtual void voltear() = 0;
    // Para saber si es carta especial sin cast
    virtual bool esFlip() const = 0;
    std::string getRutaImagen() const;
};

#endif // CARTA_H
