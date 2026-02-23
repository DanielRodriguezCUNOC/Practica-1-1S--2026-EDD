#ifndef LADOREVERSE_H
#define LADOREVERSE_H
#include "ladocarta.h"

class LadoReverse : public LadoCarta
{
public:
    LadoReverse(Color c, int n, std::string ruta);

    void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado = "", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1) override;
    LadoCarta* clonar() const override;
};

#endif // LADOREVERSE_H
