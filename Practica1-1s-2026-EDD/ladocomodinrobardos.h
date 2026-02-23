#ifndef LADOCOMODINROBARDOS_H
#define LADOCOMODINROBARDOS_H
#include "ladocarta.h"

class LadoComodinRobarDos : public LadoCarta
{
public:
    LadoComodinRobarDos(Color c, int n, std::string ruta);

    void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado = "", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1) override;
    LadoCarta* clonar() const override;
};

#endif // LADOCOMODINROBARDOS_H
