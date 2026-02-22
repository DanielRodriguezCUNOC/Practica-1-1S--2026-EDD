#ifndef LADOFLIP_H
#define LADOFLIP_H
#include "ladocarta.h"

class LadoFlip : public LadoCarta
{
public:
    LadoFlip(Color c, int n, std::string ruta);

    void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado = "", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1) override;
    LadoCarta* clonar() const override;
};

#endif // LADOFLIP_H
