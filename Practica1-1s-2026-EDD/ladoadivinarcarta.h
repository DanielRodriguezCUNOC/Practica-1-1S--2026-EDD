#ifndef LADOADIVINARCARTA_H
#define LADOADIVINARCARTA_H
#include "ladocarta.h"

class LadoAdivinarCarta : public LadoCarta
{
public:
    LadoAdivinarCarta(Color c, int n, std::string ruta);

    void aplicarEfecto(Juego* juego, const std::string& colorSeleccionado = "", const std::string jugadorSeleccionado = "", int numeroSeleccionado = -1) override;
    LadoCarta* clonar() const override;
    bool adivinoCarta(Juego* juego, std::string nombreSeleccionado, int numeroCarta, std::string colorCarta) const;
};

#endif // LADOADIVINARCARTA_H
