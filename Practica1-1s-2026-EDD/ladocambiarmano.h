#ifndef LADOCAMBIARMANO_H
#define LADOCAMBIARMANO_H
#include "ladocarta.h"

class LadoCambiarMano : public LadoCarta
{
public:
    LadoCambiarMano(Color c, int n, std::string ruta);

    void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado = "", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1) override;
    LadoCarta* clonar() const override;
};

#endif // LADOCAMBIARMANO_H
