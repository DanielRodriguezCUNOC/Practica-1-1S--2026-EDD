#ifndef LADOCOMODINROBARSEIS_H
#define LADOCOMODINROBARSEIS_H
#include "ladocarta.h"
class LadoComodinRobarSeis:public LadoCarta
{
public:
    LadoComodinRobarSeis(Color c, int n, std::string ruta);

    void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado ="", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1)override;
    LadoCarta* clonar() const override;
};

#endif // LADOCOMODINROBARSEIS_H
