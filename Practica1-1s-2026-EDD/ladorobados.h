#ifndef LADOROBADOS_H
#define LADOROBADOS_H
#include "ladocarta.h"
class LadoRobaDos:public LadoCarta
{
public:
    LadoRobaDos(Color c, int n, std::string ruta);

    void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado = "", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1) override;
    LadoCarta* clonar() const override;
};

#endif // LADOROBADOS_H
