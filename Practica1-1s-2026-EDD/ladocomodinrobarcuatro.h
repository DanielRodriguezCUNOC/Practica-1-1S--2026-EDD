#ifndef LADOCOMODINROBARCUATRO_H
#define LADOCOMODINROBARCUATRO_H
#include "ladocarta.h"
class LadoComodinRobarCuatro : public LadoCarta
{
public:
    LadoComodinRobarCuatro(Color c, int n, std::string ruta);
    void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado ="", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1)override;
    LadoCarta* clonar() const override;
};

#endif // LADOCOMODINROBARCUATRO_H
