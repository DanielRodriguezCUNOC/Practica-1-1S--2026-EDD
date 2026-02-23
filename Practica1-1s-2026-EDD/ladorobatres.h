#ifndef LADOROBATRES_H
#define LADOROBATRES_H
#include "ladocarta.h"

class LadoRobaTres : public LadoCarta
{
public:
    LadoRobaTres(Color c, int n, std::string ruta);

    void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado = "", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1) override;
    LadoCarta* clonar() const override;
};

#endif // LADOROBA3_H
