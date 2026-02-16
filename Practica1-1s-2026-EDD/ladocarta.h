#ifndef LADOCARTA_H
#define LADOCARTA_H
#include <string>
#include "utils.h"


class LadoCarta
{
private:
    TipoCarta tipo;
    Color color;
    int numero;

    // Métodos privados para generar nombres de archivo
    std::string colorToString() const;
    std::string tipoToString() const;

public:
    // Constructor (n = -1 por defecto para cartas de acción)
    LadoCarta(TipoCarta t, Color c, int n = -1);
    LadoCarta() = default; // Constructor vacío necesario para arrays/listas

    TipoCarta getTipo() const;
    Color getColor() const;
    int getNumero() const;

    // Genera: "AMARILLO", "ROSA", etc.
    std::string getNombreCarpetaColor() const;

    // Genera: "A+1", "FLIP", "5", etc.
    std::string getNombreArchivo() const;
};

#endif // LADOCARTA_H
