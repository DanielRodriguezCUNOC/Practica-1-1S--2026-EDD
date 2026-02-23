#ifndef LADODORSOGENERICO_H
#define LADODORSOGENERICO_H
#include "ladocarta.h"

class LadoDorsoGenerico : public LadoCarta
{
public:
    LadoDorsoGenerico(Color c, int n, std::string ruta);

    void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado = "", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1) override;
    LadoCarta* clonar() const override;
};

#endif // LADODORSOGENERICO_H
