#ifndef LADONUMERO_H
#define LADONUMERO_H

#include "ladocarta.h"
class LadoNumero: public LadoCarta
{
public:
    LadoNumero(Color c, int n, std::string ruta);

    void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado ="", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1)override;
    LadoCarta* clonar() const override;
};

#endif // LADONUMERO_H
