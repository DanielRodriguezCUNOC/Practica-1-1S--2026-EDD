#ifndef LADOSALTO_H
#define LADOSALTO_H
#include "ladocarta.h"

class LadoSalto : public LadoCarta
{
public:
    LadoSalto(Color c, int n, std::string ruta);

    void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado = "", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1) override;
    LadoCarta* clonar() const override;
};

#endif // LADOSALTO_H
