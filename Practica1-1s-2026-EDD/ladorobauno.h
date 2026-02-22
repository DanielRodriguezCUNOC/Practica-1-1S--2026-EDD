#ifndef LADOROBAUNO_H
#define LADOROBAUNO_H
#include "ladocarta.h"

class LadoRobaUno : public LadoCarta
{
public:
    LadoRobaUno(Color c, int n, std::string ruta);

    void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado = "", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1) override;
    LadoCarta* clonar() const override;
};

#endif // LADOROBA1_H
