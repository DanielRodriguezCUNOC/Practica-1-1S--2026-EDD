#ifndef LADOSALTOTODOS_H
#define LADOSALTOTODOS_H
#include "ladocarta.h"

class LadoSaltoTodos : public LadoCarta
{
public:
    LadoSaltoTodos(Color c, int n, std::string ruta);

    void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado = "", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1) override;
    LadoCarta* clonar() const override;
};

#endif // LADOSALTOTODOS_H
