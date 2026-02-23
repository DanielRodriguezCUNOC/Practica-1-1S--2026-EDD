#ifndef LADOCOLORETERNO_H
#define LADOCOLORETERNO_H
#include "ladocarta.h"

class LadoColorEterno : public LadoCarta
{
public:
    LadoColorEterno(Color c, int n, std::string ruta);

    void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado = "", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1) override;
    LadoCarta* clonar() const override;
};

#endif // LADOCOLORETERNO_H
