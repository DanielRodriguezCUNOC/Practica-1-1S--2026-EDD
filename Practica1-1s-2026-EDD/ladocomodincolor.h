#ifndef LADOCOMODINCOLOR_H
#define LADOCOMODINCOLOR_H
#include "ladocarta.h"
class LadoComodinColor:public LadoCarta
{
public:
    LadoComodinColor(Color c, int n, std::string ruta);

    void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado ="", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1)override;
    LadoCarta* clonar() const override;
};

#endif // LADOCOMODINCOLOR_H
