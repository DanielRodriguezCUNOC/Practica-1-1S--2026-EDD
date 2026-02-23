#ifndef LADOCAMBIARDIRECCION_H
#define LADOCAMBIARDIRECCION_H
#include "ladocarta.h"

class LadoCambiarDireccion : public LadoCarta
{
public:
    LadoCambiarDireccion(Color c, int n, std::string ruta);

    void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado = "", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1) override;
    LadoCarta* clonar() const override;
};

#endif // LADOCAMBIARDIRECCION_H
